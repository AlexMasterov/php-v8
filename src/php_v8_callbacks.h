/*
  +----------------------------------------------------------------------+
  | This file is part of the pinepain/php-v8 PHP extension.              |
  |                                                                      |
  | Copyright (c) 2015-2016 Bogdan Padalko <pinepain@gmail.com>          |
  |                                                                      |
  | Licensed under the MIT license: http://opensource.org/licenses/MIT   |
  |                                                                      |
  | For the full copyright and license information, please view the      |
  | LICENSE file that was distributed with this source or visit          |
  | http://opensource.org/licenses/MIT                                   |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_V8_CALLBACKS_H
#define PHP_V8_CALLBACKS_H

namespace phpv8 {
    class Callback;
    class CallbacksBucket;
    class PersistentData;
    template <class T> class PersistentCollection;
}

#include <v8.h>
#include <map>
#include <string>

extern "C" {
#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#endif
}


extern void php_v8_callbacks_gc(phpv8::PersistentData *data, zval **gc_data, int * gc_data_count, zval **table, int *n);

extern void php_v8_bucket_gc(phpv8::CallbacksBucket *bucket, zval **gc_data, int * gc_data_count, zval **table, int *n);

extern void php_v8_callback_function(const v8::FunctionCallbackInfo<v8::Value>& info);
extern void php_v8_callback_accessor_name_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
extern void php_v8_callback_accessor_name_setter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

extern void php_v8_callback_generic_named_property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
extern void php_v8_callback_generic_named_property_setter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
extern void php_v8_callback_generic_named_property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info);
extern void php_v8_callback_generic_named_property_deleter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Boolean>& info);
extern void php_v8_callback_generic_named_property_enumerator( const v8::PropertyCallbackInfo<v8::Array>& info);

extern void php_v8_callback_indexed_property_getter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
extern void php_v8_callback_indexed_property_setter(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
extern void php_v8_callback_indexed_property_query(uint32_t index, const v8::PropertyCallbackInfo<v8::Integer>& info);
extern void php_v8_callback_indexed_property_deleter(uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info);
extern void php_v8_callback_indexed_property_enumerator(const v8::PropertyCallbackInfo<v8::Array>& info);

extern bool php_v8_callback_access_check(v8::Local<v8::Context> accessing_context, v8::Local<v8::Object> accessed_object, v8::Local<v8::Value> data);


namespace phpv8 {

    class Callback {
    public:
        Callback(zend_fcall_info fci, zend_fcall_info_cache fci_cache);
        ~Callback();
        int getGcCount();
        void collectGcZvals(zval *& zv);

        inline zend_fcall_info fci() {
            return fci_;
        }

        inline zend_fcall_info_cache fci_cache() {
            return fci_cache_;
        }

    private:
        zval object;
        zend_fcall_info fci_;
        zend_fcall_info_cache fci_cache_;
    };


    class CallbacksBucket {
    public:
        phpv8::Callback *get(size_t index);
        void reset(CallbacksBucket *bucket);

        void add(size_t index, zend_fcall_info fci, zend_fcall_info_cache fci_cache);
        int getGcCount();

        void collectGcZvals(zval *& zv);

        inline bool empty() {
            return callbacks.empty();
        }
    private:
        std::map<size_t, std::shared_ptr<phpv8::Callback>> callbacks;
    };


    class PersistentData {
    public:
        int getGcCount();
        void collectGcZvals(zval *& zv);
        CallbacksBucket *bucket(const char *prefix, bool is_symbol, const char *name);

        inline CallbacksBucket *bucket(const char *name) {
            return bucket("", false, name);
        }

        inline bool empty() {
            return buckets.empty();
        }

    private:
        std::map<std::string, std::shared_ptr<CallbacksBucket>> buckets;
    };


    template <class T>
    class PersistentCollection {
    public:
        ~PersistentCollection() {
            for (auto const &item : collection) {
                item.first->Reset();
                delete item.first;
            }
        }

        int getGcCount() {
            int size = 0;

            for (auto const &item : collection) {
                size += item.second->getGcCount();
            }

            return size;
        }

        void collectGcZvals(zval *& zv) {
            for (auto const &item : collection) {
                item.second->collectGcZvals(zv);
            }
        }

        void add(v8::Persistent<T> *persistent, phpv8::PersistentData *data) {
            collection[persistent] = std::shared_ptr<phpv8::PersistentData>(data);
        }

        void remove(v8::Persistent<T, v8::NonCopyablePersistentTraits<T>> *persistent) {
            collection.erase(persistent);
        }    private:
        std::map<v8::Persistent<T> *, std::shared_ptr<phpv8::PersistentData>> collection;
    };
}


#endif //PHP_V8_CALLBACKS_H

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
