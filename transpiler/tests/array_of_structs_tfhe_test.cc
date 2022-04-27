// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ios>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "transpiler/tests/struct_tfhe.h"
#include "transpiler/tests/test_util.h"
#include "xls/common/status/matchers.h"

namespace fully_homomorphic_encryption {
namespace transpiler {
namespace {

using ::fully_homomorphic_encryption::transpiler::TranspilerTestBase;

template <typename InputType, typename OutputType>
struct TranspilerControlStructureTestCase {
  const std::string test_name;
  const InputType input;
  const OutputType expected_output;
};
using TranspilerControlStructureSwitchTestCase =
    TranspilerControlStructureTestCase<char, int>;

class TfheArrayOfStructsTest : public TranspilerTestBase {};

TEST_F(TfheArrayOfStructsTest, DynamicOneDimArray) {
  Struct array[2] = {{'a', (short)0x1234, (int)0x789abcde},
                     {'b', (short)0x5678, (int)0xc0deba7e}};

  // Dynamic array with 2 elements.
  TfheArray<Struct> dyn_one_dim(2, params());
  EXPECT_EQ(dyn_one_dim.length(), 2);
  EXPECT_EQ(dyn_one_dim.bit_width(), 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(dyn_one_dim.get().size(), dyn_one_dim.bit_width());

  dyn_one_dim.SetEncrypted(array, 2, secret_key());
  Struct decoded[2];
  dyn_one_dim.Decrypt(decoded, 2, secret_key());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(array[i].c, decoded[i].c);
    EXPECT_EQ(array[i].s, decoded[i].s);
    EXPECT_EQ(array[i].i, decoded[i].i);
  }

  absl::FixedArray<Struct> another_decoded = dyn_one_dim.Decrypt(secret_key());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(another_decoded[i].c, decoded[i].c);
    EXPECT_EQ(another_decoded[i].s, decoded[i].s);
    EXPECT_EQ(another_decoded[i].i, decoded[i].i);
  }

  TfheStructRef ref = dyn_one_dim[1];
  EXPECT_EQ(ref.length(), 1);
  EXPECT_EQ(ref.bit_width(), (1 + 2 + 4) * 8);
  EXPECT_EQ(ref.get().size(), ref.bit_width());

  TfheStructRef ref_ref = ref;
  EXPECT_EQ(ref_ref.length(), 1);
  EXPECT_EQ(ref_ref.bit_width(), (1 + 2 + 4) * 8);
  EXPECT_EQ(ref_ref.get().size(), ref_ref.bit_width());

  Struct decoded_ref = ref.Decrypt(secret_key());
  EXPECT_EQ(decoded_ref.c, decoded[1].c);
  EXPECT_EQ(decoded_ref.s, decoded[1].s);
  EXPECT_EQ(decoded_ref.i, decoded[1].i);

  {
    // Get a reference to the whole array.
    TfheArrayRef<Struct> dyn_one_dim_ref = dyn_one_dim;
    EXPECT_EQ(dyn_one_dim_ref.length(), 2);
    EXPECT_EQ(dyn_one_dim_ref.bit_width(), 2 * (1 + 2 + 4) * 8);
    EXPECT_EQ(dyn_one_dim_ref.get().size(), dyn_one_dim_ref.bit_width());

    // Check the array again via the reference.
    Struct decoded_via_ref[2];
    dyn_one_dim.Decrypt(decoded_via_ref, 2, secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, decoded[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, decoded[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, decoded[i].i);
    }
    absl::FixedArray<Struct> another_decoded_via_ref =
        dyn_one_dim.Decrypt(secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, another_decoded_via_ref[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, another_decoded_via_ref[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, another_decoded_via_ref[i].i);
    }

    TfheStructRef ref_via_ref = dyn_one_dim_ref[1];
    EXPECT_EQ(ref_via_ref.length(), 1);
    EXPECT_EQ(ref_via_ref.bit_width(), (1 + 2 + 4) * 8);
    EXPECT_EQ(ref_via_ref.get().size(), ref_via_ref.bit_width());
    Struct decoded_ref_via_ref = ref_via_ref.Decrypt(secret_key());
    EXPECT_EQ(decoded_ref_via_ref.c, decoded[1].c);
    EXPECT_EQ(decoded_ref_via_ref.s, decoded[1].s);
    EXPECT_EQ(decoded_ref_via_ref.i, decoded[1].i);
  }

  // Repeat but use the TfheStructArrayRef<> type
  {
    TfheStructArrayRef<> dyn_one_dim_ref = dyn_one_dim;
    EXPECT_EQ(dyn_one_dim_ref.length(), 2);
    EXPECT_EQ(dyn_one_dim_ref.bit_width(), 2 * (1 + 2 + 4) * 8);
    EXPECT_EQ(dyn_one_dim_ref.get().size(), dyn_one_dim_ref.bit_width());

    // Check the array again via the reference.
    Struct decoded_via_ref[2];
    dyn_one_dim.Decrypt(decoded_via_ref, 2, secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, decoded[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, decoded[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, decoded[i].i);
    }
    TfheStructRef ref_via_ref = dyn_one_dim_ref[1];
    EXPECT_EQ(ref_via_ref.length(), 1);
    EXPECT_EQ(ref_via_ref.bit_width(), (1 + 2 + 4) * 8);
    EXPECT_EQ(ref_via_ref.get().size(), ref_via_ref.bit_width());
    Struct decoded_ref_via_ref = ref_via_ref.Decrypt(secret_key());
    EXPECT_EQ(decoded_ref_via_ref.c, decoded[1].c);
    EXPECT_EQ(decoded_ref_via_ref.s, decoded[1].s);
    EXPECT_EQ(decoded_ref_via_ref.i, decoded[1].i);
  }
}

TEST_F(TfheArrayOfStructsTest, FixedWidthOneDimArray) {
  Struct array[2] = {{'a', (short)0x1234, (int)0x789abcde},
                     {'b', (short)0x5678, (int)0xc0deba7e}};

  // Static array with 2 elements.
  // TfheArray<Struct, void, 2> fixed_one_dim;
  TfheStructArray<2> fixed_one_dim(params());
  EXPECT_EQ(fixed_one_dim.length(), 2);
  EXPECT_EQ(fixed_one_dim.bit_width(), 2 * (1 + 2 + 4) * 8);

  fixed_one_dim.SetEncrypted(array, secret_key());

  Struct decoded[2];
  fixed_one_dim.Decrypt(decoded, secret_key());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(array[i].c, decoded[i].c);
    EXPECT_EQ(array[i].s, decoded[i].s);
    EXPECT_EQ(array[i].i, decoded[i].i);
  }

  absl::FixedArray<Struct> another_decoded =
      fixed_one_dim.Decrypt(secret_key());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(another_decoded[i].c, decoded[i].c);
    EXPECT_EQ(another_decoded[i].s, decoded[i].s);
    EXPECT_EQ(another_decoded[i].i, decoded[i].i);
  }

  TfheStructRef ref = fixed_one_dim[1];
  Struct decoded_ref = ref.Decrypt(secret_key());
  EXPECT_EQ(decoded_ref.c, decoded[1].c);
  EXPECT_EQ(decoded_ref.s, decoded[1].s);
  EXPECT_EQ(decoded_ref.i, decoded[1].i);

  {
    // Get a reference to the whole array.
    TfheStructArrayRef<2> fixed_one_dim_ref = fixed_one_dim;
    EXPECT_EQ(fixed_one_dim_ref.length(), 2);
    EXPECT_EQ(fixed_one_dim_ref.bit_width(), 2 * (1 + 2 + 4) * 8);

    // Check the array again via the reference.
    Struct decoded_via_ref[2];
    fixed_one_dim.Decrypt(decoded_via_ref, secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, decoded[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, decoded[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, decoded[i].i);
    }

    absl::FixedArray<Struct> another_decoded_via_ref =
        fixed_one_dim.Decrypt(secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, another_decoded_via_ref[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, another_decoded_via_ref[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, another_decoded_via_ref[i].i);
    }
  }

  // Do the same using alternate declaration.
  {
    // Get a reference to the whole array.
    TfheArrayRef<Struct, void, 2> fixed_one_dim_ref = fixed_one_dim;
    EXPECT_EQ(fixed_one_dim_ref.length(), 2);
    EXPECT_EQ(fixed_one_dim_ref.bit_width(), 2 * (1 + 2 + 4) * 8);

    // Check the array again via the reference.
    Struct decoded_via_ref[2];
    fixed_one_dim.Decrypt(decoded_via_ref, secret_key());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i].c, decoded[i].c);
      EXPECT_EQ(decoded_via_ref[i].s, decoded[i].s);
      EXPECT_EQ(decoded_via_ref[i].i, decoded[i].i);
    }
  }
}

TEST_F(TfheArrayOfStructsTest, FixedWidth4x3x2Array) {
  Struct array[4][3][2] = {{{{'a', (short)0x1234, (int)0x789abcde},
                             {'b', (short)0x5678, (int)0xc0deba7e}},
                            {{'c', (short)0x3412, (int)0xbcde789a},
                             {'d', (short)0x7856, (int)0xba7ec0de}},
                            {{'e', (short)0x4231, (int)0xde9abc78},
                             {'f', (short)0x8675, (int)0x7edebac0}}},

                           {{{'g', (short)0x9abc, (int)0x11223344},
                             {'h', (short)0xdef0, (int)0x55667788}},
                            {{'i', (short)0xbc9a, (int)0x33441122},
                             {'j', (short)0xf0de, (int)0x77885566}},
                            {{'k', (short)0xcab9, (int)0x44223311},
                             {'l', (short)0x0efd, (int)0x7edebac0}}},

                           {{{'m', (short)0x1122, (int)0x99aabbcc},
                             {'n', (short)0x3344, (int)0xddeeff00}},
                            {{'o', (short)0x2211, (int)0xbbcc99aa},
                             {'p', (short)0x4433, (int)0xff00ddee}},
                            {{'q', (short)0x2121, (int)0xccaabb99},
                             {'r', (short)0x3434, (int)0x00eeffdd}}},

                           {{{'s', (short)0x5566, (int)0x12345678},
                             {'t', (short)0x7788, (int)0x9abcdef0}},
                            {{'u', (short)0x6655, (int)0x56781234},
                             {'v', (short)0x8877, (int)0xdef09abc}},
                            {{'w', (short)0x5656, (int)0x78345612},
                             {'x', (short)0x7878, (int)0xf0bcde9a}}}};

  // Static array with 4x3x2 elements.
  // Equivalent declaration: TfheArray<Struct, void, 4, 3, 2> fixed_3x2dim;
  TfheStructArray<4, 3, 2> fixed_4x3x2dim(params());
  EXPECT_EQ(fixed_4x3x2dim.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim.bit_width(), 4 * 3 * 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(fixed_4x3x2dim.get().size(), fixed_4x3x2dim.bit_width());
  fixed_4x3x2dim.SetEncrypted(array, secret_key());
  Struct decoded[4][3][2];
  fixed_4x3x2dim.Decrypt(decoded, secret_key());
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        EXPECT_EQ(array[i][j][k].c, decoded[i][j][k].c);
        EXPECT_EQ(array[i][j][k].s, decoded[i][j][k].s);
        EXPECT_EQ(array[i][j][k].i, decoded[i][j][k].i);
      }
    }
  }

  // Get a reference to the whole array and check it.
  TfheStructArrayRef<4, 3, 2> fixed_4x3x2dim_ref = fixed_4x3x2dim;
  EXPECT_EQ(fixed_4x3x2dim_ref.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim_ref.bit_width(), 4 * 3 * 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(fixed_4x3x2dim_ref.get().size(), fixed_4x3x2dim_ref.bit_width());
  Struct decoded_via_ref[4][3][2];
  fixed_4x3x2dim_ref.Decrypt(decoded_via_ref, secret_key());
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        EXPECT_EQ(array[i][j][k].c, decoded_via_ref[i][j][k].c);
        EXPECT_EQ(array[i][j][k].s, decoded_via_ref[i][j][k].s);
        EXPECT_EQ(array[i][j][k].i, decoded_via_ref[i][j][k].i);
      }
    }
  }

  // Obtain a copy of the whole-array reference
  TfheStructArrayRef<4, 3, 2> fixed_4x3x2dim_ref_ref = fixed_4x3x2dim_ref;
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.bit_width(), 4 * 3 * 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.get().size(),
            fixed_4x3x2dim_ref_ref.bit_width());

  // Obtain a subarray from the array by subscript and check it.
  // TODO: replace this with auto and static-assert the type
  TfheStructArrayRef<3, 2> fixed_3x2dim_ref = fixed_4x3x2dim[1];
  EXPECT_EQ(fixed_3x2dim_ref.length(), 3);
  Struct subarray_3x2[3][2];
  fixed_3x2dim_ref.Decrypt(subarray_3x2, secret_key());
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 2; k++) {
      EXPECT_EQ(array[1][j][k].c, subarray_3x2[j][k].c);
      EXPECT_EQ(array[1][j][k].s, subarray_3x2[j][k].s);
      EXPECT_EQ(array[1][j][k].i, subarray_3x2[j][k].i);
    }
  }

  // Obtain a subarray from the subarray via the reference, and check it.
  TfheStructArrayRef<2> fixed_2dim_ref = fixed_3x2dim_ref[2];
  EXPECT_EQ(fixed_2dim_ref.length(), 2);
  EXPECT_EQ(fixed_2dim_ref.bit_width(), 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(fixed_2dim_ref.get().size(), fixed_2dim_ref.bit_width());
  Struct subarray_2[2];
  fixed_2dim_ref.Decrypt(subarray_2, secret_key());
  for (int k = 0; k < 2; k++) {
    EXPECT_EQ(array[1][2][k].c, subarray_2[k].c);
    EXPECT_EQ(array[1][2][k].s, subarray_2[k].s);
    EXPECT_EQ(array[1][2][k].i, subarray_2[k].i);
  }

  // Obtain the subarray directly from the array.
  TfheStructArrayRef<2> fixed_2dim_direct_ref = fixed_4x3x2dim[1][2];
  EXPECT_EQ(fixed_2dim_direct_ref.length(), 2);
  EXPECT_EQ(fixed_2dim_direct_ref.bit_width(), 2 * (1 + 2 + 4) * 8);
  EXPECT_EQ(fixed_2dim_direct_ref.get().size(),
            fixed_2dim_direct_ref.bit_width());
  Struct subarray_direct_2[2];
  fixed_2dim_direct_ref.Decrypt(subarray_direct_2, secret_key());
  for (int k = 0; k < 2; k++) {
    EXPECT_EQ(array[1][2][k].c, subarray_direct_2[k].c);
    EXPECT_EQ(array[1][2][k].s, subarray_direct_2[k].s);
    EXPECT_EQ(array[1][2][k].i, subarray_direct_2[k].i);
  }
}

}  // namespace
}  // namespace transpiler
}  // namespace fully_homomorphic_encryption
