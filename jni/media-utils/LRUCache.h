#ifndef _LRU_CACHE_H_
#define _LRU_CACHE_H_

#include <cstddef>
#include <list>
#include <stdexcept>
#include <utility>
#include <unordered_map>

namespace laiwang {
namespace Utils {


template<typename Key, typename Value>
class LRUCache {
public:
    typedef typename std::pair<Key, Value> CacheEntry;
    typedef typename std::list<CacheEntry>::iterator CacheEntryIterator;

    LRUCache(size_t size): max_size_(size) {}

    void put(const Key& key, const Value& value) {
        auto it = cache_items_index_.find(key);
        if (it != cache_items_index_.end()) {
            // if key exists, erase the old cache item and cache item index.
            cache_items_.erase(it->second);
            cache_items_index_.erase(it);
        }

        // insert the new cache item and cache item index.
        cache_items_.push_front(CacheEntry(key, value));
        cache_items_index_[key] = cache_items_.begin();

        // if no enough space for any new cache item, evict the last one.
        if (cache_items_.size() > max_size_) {
            auto last = cache_items_.end();
            last--;
            cache_items_index_.erase(last->first);
            cache_items_.pop_back();
        }
    }

    const Value& get(const Key& key) {
        auto it = cache_items_index_.find(key);
        if (it == cache_items_index_.end()) {
//            throw std::range_error("No such key in the cache");
            return NULL;
        } else {
            cache_items_.splice(cache_items_.begin(), cache_items_, it->second);
            return it->second->second;
        }
    }

    bool exists(const Key& key) const {
        return cache_items_index_.find(key) != cache_items_index_.end();
    }

    size_t size() const {
        return cache_items_index_.size();
    }

private:
    std::list<CacheEntry> cache_items_;
    std::unordered_map<Key, CacheEntryIterator> cache_items_index_;
    size_t max_size_;
};

}
}

#endif
