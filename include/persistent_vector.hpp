#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>


/**
 * An attempt at implementing a persistent vector
 */
template<
   typename value_type
>
class persistent_vector
{
private:
   static constexpr std::size_t BlockBits = 5;
   static constexpr std::size_t BlockSize = 1 << BlockBits;

   enum class kind : char { leaf = 0, intern = 1 };

   struct node
   {
      node(): node(kind::leaf) {}
      node(kind k) : m_kind(k) {}
      const kind m_kind;
   };

   using node_ptr = std::shared_ptr<node>;

   struct leaf_node : node
   {
      leaf_node(): node(kind::leaf), m_values() {}
      std::vector<value_type> m_values;
   };

   struct intern_node : node
   {
      intern_node(): node(kind::intern), m_first(0), m_last(0), m_height(1), m_childs() {}
      std::size_t             m_first;
      std::size_t             m_last;
      std::size_t             m_height; //Height of the sub-tree
      std::vector<node_ptr>   m_childs;
   };

public:
   persistent_vector() : m_root(new leaf_node) {}
   ~persistent_vector() = default;
   persistent_vector(persistent_vector const&) = default;
   persistent_vector& operator=(persistent_vector const&) = default;

   std::size_t size() const
   {
      return get_size(m_root.get());
   }

   value_type const& at(std::size_t index) const
   {
      if (index >= size())
         throw std::exception("Wrong size");
      return get_at(m_root, index);
   }

   persistent_vector push_back(value_type const& v) const
   {
      return persistent_vector(push_back(m_root, v));
   }

private:
   node_ptr m_root;

   persistent_vector(node_ptr const& r) : m_root(r) {}

   static std::size_t get_size(node const* n)
   {
      if (!n) return 0;
      if (n->m_kind == kind::leaf)
         return static_cast<leaf_node const*>(n)->m_values.size();

      intern_node const* in = static_cast<intern_node const*>(n);
      return in->m_last - in->m_first;
   }

   static std::size_t max_size(node const* n)
   {
      if (!n) return 0;
      if (n->m_kind == kind::leaf) return BlockSize;

      auto* i = static_cast<intern_node const*>(n);
      return 1 << (BlockBits * (1 + i->m_height));
   }

   static node_ptr create_leaf(value_type const& v)
   {
      std::shared_ptr<leaf_node> out(new leaf_node());
      out->m_values.push_back(v);
      return out;
   }

   //--------------------------------------------------------------------------

   static value_type const& get_at(node_ptr const& n, std::size_t index)
   {
      if (n->m_kind == kind::leaf)
         return static_cast<leaf_node const*>(n.get())->m_values[index];

      //TODO - Could be done by just dividing when updating
      //TODO - Could be done by selecting the last one when push back
      auto* i = static_cast<intern_node const*>(n.get());
      for (auto const& n : i->m_childs)
      {
         auto size = get_size(n.get());
         if (index < size)
            return get_at(n, index);
         index -= size;
      }
   }

   //--------------------------------------------------------------------------

   static node_ptr push_back(node_ptr const& n, value_type const& v)
   {
      return n->m_kind == kind::leaf ? push_in_leaf(n, v) : push_in_intern(n, v);
   }

   static node_ptr push_in_leaf(node_ptr const& n, value_type const& v)
   {
      auto* l = static_cast<leaf_node const*>(n.get());

      if (l->m_values.size() >= BlockSize)
      {
         std::shared_ptr<intern_node> out(new intern_node());
         out->m_first = 0;
         out->m_last = BlockSize + 1;
         out->m_childs.push_back(n);
         out->m_childs.push_back(create_leaf(v));
         return std::move(out);
      }

      std::shared_ptr<leaf_node> out(new leaf_node(*l));
      out->m_values.push_back(v);
      return std::move(out);
   }

   static node_ptr push_in_intern(node_ptr const& n, value_type const& v)
   {
      auto* i = static_cast<intern_node const*>(n.get());

      std::size_t first_size = max_size(i->m_childs[0].get());
      auto it = find_if(begin(i->m_childs), end(i->m_childs),
         [&](auto const& n) { return first_size > get_size(n.get()); });

      if (it != end(i->m_childs))
      {
         auto index = distance(begin(i->m_childs), it);
         std::shared_ptr<intern_node> out(new intern_node(*i));
         out->m_last++;
         out->m_childs[index] = push_back(*it, v);
         return std::move(out);
      }
      else if (i->m_childs.size() < BlockSize)
      {
         std::shared_ptr<intern_node> out(new intern_node(*i));
         out->m_last++;
         out->m_childs.push_back(create_leaf(v));
         return std::move(out);
      }
      else
      {
         std::shared_ptr<intern_node> out(new intern_node());
         out->m_last = 1 + i->m_last;
         out->m_height = 1 + i->m_height;
         out->m_childs.push_back(n);
         out->m_childs.push_back(create_leaf(v));
         return out;
      }
   }
};
