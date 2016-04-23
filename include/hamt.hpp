#pragma once

#include <algorithm>
#include <bitset>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>


/**
 * An attempt at implementing a hash array map trie
 */
template<
   typename key_type,
   typename value_type
>
class hash_array_mapped_trie
{
private:
   static constexpr std::size_t BitCount = 5;
   static constexpr std::size_t FlagSize = 1 << BitCount;
   static constexpr std::size_t BitHMask = FlagSize - 1;
   static constexpr std::size_t HashSize = sizeof(std::size_t) * 8;
   static constexpr std::size_t MaxDepth = HashSize / BitCount + (HashSize % BitCount ? 1 : 0);

   using leaf = std::pair<key_type, value_type>;

   struct node
   {
      node() : m_flags(0), m_childs(), m_size(0), m_leaves() {}
      std::bitset<FlagSize> m_flags;
      std::vector<node>     m_childs;
      std::size_t           m_size;
      std::vector<leaf>     m_leaves;
   };

public:
   hash_array_mapped_trie() = default;
   ~hash_array_mapped_trie() = default;

   bool insert(key_type const& key, value_type const& value)
   {
      return insert(key, value, std::hash<key_type>()(key));
   }

   std::pair<bool, value_type> find(key_type const& key) const
   {
      return find(key, std::hash<key_type>()(key));
   }

   std::size_t size() const
   {
      return m_root.m_size;
   }

   bool contains(key_type const& key) const
   {
      return find(key).first;
   }

   bool empty() const
   {
      return 0 == size();
   }

private:
   node m_root;

   bool insert(key_type const& k, value_type const& v, std::size_t h)
   {
      return insert_rec(m_root, k, v, h, MaxDepth);
   }

   static bool insert_rec(node& current, key_type const& k, value_type const& v, std::size_t h, size_t depth)
   {
      if (depth == 0)
      {
         auto it = find_leaf(current, k);
         if (it != end(current.m_leaves))
         {
            it->second = v;
            return false;
         }
         else
         {
            current.m_leaves.push_back({ k, v });
            current.m_size++;
            return true;
         }
      }
      
      std::size_t node_hash = h & BitHMask;
      std::size_t index = index_of(current, node_hash);
      if (!is_there(current, node_hash))
      {
         current.m_flags.set(node_hash);
         current.m_childs.insert(begin(current.m_childs) + index, node());
      }

      bool added = insert_rec(current.m_childs[index], k, v, h >> BitCount, depth-1);
      if (added) current.m_size++;
      return added;
   }

   std::pair<bool, value_type> find(key_type const& k, std::size_t h) const
   {
      static const std::pair<bool, value_type> NotFound(false, value_type());
      
      node const* current = &m_root;
      for (size_t i = 0; i < MaxDepth; ++i, h = h >> BitCount)
      {
         std::size_t node_hash = h & BitHMask;
         if (!is_there(*current, node_hash))
            return NotFound;

         std::size_t index = index_of(*current, node_hash);
         current = &(current->m_childs[index]);
      }

      auto it = find_leaf(*current, k);
      return it == end(current->m_leaves) ? NotFound : std::make_pair(true, it->second);
   }

   static bool is_there(node const& n, std::size_t h)
   {
      return n.m_flags.test(h);
   }

   static std::size_t index_of(node const& n, std::size_t h)
   {
      std::size_t shift = FlagSize - h;
      std::bitset<FlagSize> shifted = n.m_flags << shift; //And not (>>)
      return shifted.count();
   }

   static auto find_leaf(node& n, key_type const& k)
   {
      return find_if(begin(n.m_leaves), end(n.m_leaves), [&](auto const& p) { return p.first == k; });
   }

   static auto find_leaf(node const& n, key_type const& k)
   {
      return find_if(begin(n.m_leaves), end(n.m_leaves), [&](auto const& p) { return p.first == k; });
   }
};
