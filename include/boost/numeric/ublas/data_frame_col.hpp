#ifndef _BOOST_UBLAS_DATA_FRAME_COL_	
#define _BOOST_UBLAS_DATA_FRAME_COL_
#include <boost/numeric/ublas/vector.hpp>
#include <boost/poly_collection/base_collection.hpp>
#include <unordered_map>
#include <functional>
namespace boost { namespace numeric { namespace ublas {	


class data_frame_col {
public:
    template<typename T>
    using store_type = boost::numeric::ublas::vector<T>;
    data_frame_col() = default;

    template<typename T>
    void setSize(int rows) {
        if (vals<T>.find(this) == std::end(vals<T>))
        {   
            clear_functions.emplace_back([](data_frame_col& _c){vals<T>.erase(&_c);});
            copy_functions.emplace_back([](const data_frame_col& _from, data_frame_col& _to) {
                vals<T>[&_to] = vals<T>[&_from];
            });
            size_functions.emplace_back([](const data_frame_col& _c){return vals<T>[&_c].size();});
        }
        vals<T>[this] = store_type<T>(rows);
    }
    data_frame_col(const data_frame_col& _other) {
        *this = _other;
    }
    ~data_frame_col() {
        clear();
    }
    template<typename T>
    store_type<T>& get_vector();
    template<typename T>
    const store_type<T>& get_vector() const;
    template<typename T>
    T &at(size_t idx);
    template<typename T>
    const T &at(size_t idx) const;
    
private:
    void clear() {
        for (auto&& clear_func : clear_functions) {
            clear_func(*this);
        }
    }
    
    template<class...>
    struct type_list{};
    template<class... TypeLists>
    struct visitor_base {
        using types = boost::numeric::ublas::data_frame_col::type_list<TypeLists...>;
    };
    template<class T>
    static std::unordered_map<const data_frame_col*, store_type<T>> vals;

    std::vector<std::function<void(data_frame_col&)>> clear_functions;
    std::vector<std::function<void(const data_frame_col&, data_frame_col&)>> copy_functions;
    std::vector<std::function<size_t(const data_frame_col&)>> size_functions;
};
template<class T>
std::unordered_map<const data_frame_col*, typename data_frame_col::store_type<T>> data_frame_col::vals;
}}}



#endif