#ifndef FS_REFCOUNT_MEMORY_POLICY
#define FS_REFCOUNT_MEMORY_POLICY

#include <unordered_map>
#include <functional>
#include <limits>
#include <cassert>

namespace FlatScene {

	template <typename Resource,typename CountIndex = unsigned int>
	class RefCountMemoryPolicy {
	public:
		typedef std::unordered_map<Resource*,CountIndex> CountMap;
		typedef Resource* Holder;

		static inline const CountMap& getCounts() {
			return _count;
		}

		static inline Holder clone(Holder res) {
			_count[res]++;
			assert(_count[res] != std::numeric_limits<CountIndex>::min());
			return res;
		}

		static inline void destroy(Holder res) {
			auto it = _count.find(res);
			assert(it != _count.end());
			it->second--;
			if (it->second == 0) {
				_count.erase(res);
				delete res;
			}
		}

	    static inline bool isSame(Holder lhs, Holder rhs) {
	        return lhs == rhs;
	    }

	    static inline void reset(Holder& res,Holder newval) {
	        destroy(res);
	        res = newval;
	    }

	private:
		static CountMap _count;
	};

	template <typename Resource,typename CountIndex>
	typename std::unordered_map<Resource*,CountIndex>
	RefCountMemoryPolicy<Resource,CountIndex>::_count;

} // FlatScene

#endif