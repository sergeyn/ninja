// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "util.h"

#include "test.h"

TEST(CanonicalizePath, PathSamples) {
  std::string path = "foo.h";
  std::string err;
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("foo.h", path);

  path = "./foo.h"; err = "";
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("foo.h", path);

  path = "./foo/./bar.h"; err = "";
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("foo" + string(1, kPathSeparator) + "bar.h", path);

  path = "./x/foo/../bar.h"; err = "";
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("x" + string(1, kPathSeparator) + "bar.h", path);

  path = "./x/foo/../../bar.h"; err = "";
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("bar.h", path);

  path = "foo//bar"; err = "";
  EXPECT_TRUE(CanonicalizePath(&path, &err));
  EXPECT_EQ("", err);
  EXPECT_EQ("foo" + string(1, kPathSeparator) + "bar", path);

  path = "./x/../foo/../../bar.h"; err = "";
  EXPECT_FALSE(CanonicalizePath(&path, &err));
  EXPECT_EQ("can't canonicalize path '." + string(1, kPathSeparator) + "x" + string(1, kPathSeparator) + ".." + string(1, kPathSeparator)
      + "foo" + string(1, kPathSeparator) + ".." + string(1, kPathSeparator) + ".." + string(1, kPathSeparator) + "bar.h' that reaches "
            "above its directory", err);
}
