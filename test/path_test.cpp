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
 * File:   path_test.cpp
 * Author: alex
 *
 * Created on September 6, 2016, 6:12 PM
 */

#include "staticlib/tinydir/operations.hpp"

#include <cstring>
#include <iostream>

#include "staticlib/config.hpp"
#include "staticlib/utils.hpp"

#include "staticlib/config/assert.hpp"

void test_file() {
    // create dir
    auto dir = std::string("path_test");
    sl::tinydir::create_directory(dir);
    auto tdir = sl::tinydir::path(dir);
    auto deferred = sl::support::defer([tdir]() STATICLIB_NOEXCEPT{
        tdir.remove_quietly();
    });
    
    auto filename = dir + "/tmp.file";
    auto file = sl::tinydir::path(filename);
    slassert(!file.is_directory());
    slassert(!file.is_regular_file());
    slassert(!file.exists());
    {
        auto fd = file.open_write();
        fd.write({"foo", 3});
    }
    auto tfile = sl::tinydir::path(filename);
    auto deferred2 = sl::support::defer([tfile]() STATICLIB_NOEXCEPT{
        tfile.remove_quietly();
    });
    auto nfile = sl::tinydir::path(filename);
    slassert(!nfile.is_directory());
    slassert(nfile.is_regular_file());
    slassert(nfile.exists());
}

int main() {
    try {
        test_file();
        slassert(!sl::tinydir::path("path_test").exists());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
