/*
 * Copyright 2016, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   tinydir_operations_test.cpp
 * Author: alex
 *
 * Created on September 6, 2016, 12:34 PM
 */

#include "staticlib/tinydir/operations.hpp"

#include <cstring>
#include <iostream>

#include "staticlib/config.hpp"

#include "staticlib/config/assert.hpp"

void test_list() {
    auto vec = sl::tinydir::list_directory(".");
    slassert(vec.size() > 0);
}

void test_mkdir() {
    {
        auto name = std::string("operations_test_dir");
        sl::tinydir::create_directory(name);
        auto tf = sl::tinydir::path(name);
        auto deferred = sl::support::defer([tf]() STATICLIB_NOEXCEPT {
            tf.remove_quietly();
        });
        slassert(tf.exists());
        slassert(tf.is_directory());
        slassert(!tf.is_regular_file());
        auto vec = sl::tinydir::list_directory(name);
        slassert(0 == vec.size());
    }
    slassert(!sl::tinydir::path("operations_test_dir").exists());
}

void test_normalize() {
    slassert("/foo/bar" == sl::tinydir::normalize_path("/foo/bar/"))
    slassert("/foo/bar" == sl::tinydir::normalize_path("/foo//bar"))
    slassert("c:/foo/bar" == sl::tinydir::normalize_path("c:\\foo\\bar"))
    slassert("/foo/bar" == sl::tinydir::normalize_path("/foo/./bar"))
    slassert("" == sl::tinydir::normalize_path(""))
    slassert("/" == sl::tinydir::normalize_path("/"))
}

void test_full_path() {
    auto path = std::string(".");
    auto full = sl::tinydir::full_path(path);
    slassert(full.length() > path.length());
}

int main() {
    try {
        test_list();
        test_mkdir();
        test_normalize();
        test_full_path();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
