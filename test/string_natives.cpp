/*  EMVM - Process Virtual Machine
**  Copyright (C) 2018  Zach Perkitny
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gtest/gtest.h"

#include "emvm/heap.h"
#include "emvm/natives/string.h"

namespace {

    TEST(NativeStringTest, Format) {
        emvm::Heap heap;

        emvm::String* self = heap.allocate<emvm::String>("{0}, {1}!");
        emvm::String* arg1 = heap.allocate<emvm::String>("hello");
        emvm::String* arg2 = heap.allocate<emvm::String>("world");

        emvm::Object* res = emvm::natives::string_format(
            &heap,
            { self, arg1, arg2 });
        EXPECT_EQ(res->as_str(), "hello, world!");
    }

} // namespace
