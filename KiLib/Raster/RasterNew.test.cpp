/**
 *  Copyright (c) 2020-2021 CoSci LLC, USA <software@cosci-llc.com>
 *
 *  This file is part of KiLib-OSS.
 *
 *  KiLib-OSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KiLib-OSS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KiLib-OSS.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <Eigen/Eigen>
#include <KiLib/Raster/RasterNew.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <numeric>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace KiLib
{
   TEST(RasterNew, RasterNew)
   {
      RasterNew rast = RasterNew();

      ASSERT_EQ(rast.nodata_value, RASTER_DEFAULT_NODATA_VALUE);

      ASSERT_EQ(rast.nRows, 0);
      ASSERT_EQ(rast.nCols, 0);
      ASSERT_EQ(rast.data.rows(), 0);
      ASSERT_EQ(rast.data.cols(), 0);
   }

   TEST(RasterNew, RasterNewSized)
   {
      RasterNew rast = RasterNew(10, 10);

      ASSERT_EQ(rast.nodata_value, RASTER_DEFAULT_NODATA_VALUE);

      ASSERT_EQ(rast.nRows, 10);
      ASSERT_EQ(rast.nCols, 10);
      ASSERT_EQ(rast.data.rows(), 10);
      ASSERT_EQ(rast.data.cols(), 10);
   }

   TEST(RasterNew, Resize)
   {
      RasterNew rast = RasterNew();

      rast.Resize(10, 7);
      ASSERT_EQ(rast.nRows, 10);
      ASSERT_EQ(rast.nCols, 7);
      ASSERT_EQ(rast.data.rows(), rast.nRows);
      ASSERT_EQ(rast.data.cols(), rast.nCols);

      rast.Resize(2, 15);
      ASSERT_EQ(rast.nRows, 2);
      ASSERT_EQ(rast.nCols, 15);
      ASSERT_EQ(rast.data.rows(), rast.nRows);
      ASSERT_EQ(rast.data.cols(), rast.nCols);
   }

   TEST(RasterNew, at)
   {
      RasterNew rast = RasterNew(10, 10);
      rast.at(3, 3)  = 1.0;

      ASSERT_DOUBLE_EQ(rast.at(3, 3), 1.0);
      ASSERT_DOUBLE_EQ(rast.data(3, 3), 1.0);
   }

   TEST(RasterNew, IndexOperator)
   {
      RasterNew rast = RasterNew(10, 10);
      rast(3, 3)     = 1.0;

      ASSERT_DOUBLE_EQ(rast.at(3, 3), 1.0);
      ASSERT_DOUBLE_EQ(rast.data(3, 3), 1.0);
   }

   TEST(RasterNew, flattenIndex)
   {
      RasterNew rast = RasterNew(10, 7);

      ASSERT_EQ(rast.flattenIndex(0, 3), 3);
      ASSERT_EQ(rast.flattenIndex(1, 3), 10);
      ASSERT_EQ(rast.flattenIndex(2, 9), 23);
   }

   TEST(RasterNew, OOBAccess)
   {
      RasterNew rast = RasterNew(10, 7);

      ASSERT_DEATH(rast.at(10, 7), "");
      ASSERT_DEATH(rast.at(0, 7), "");
      ASSERT_DEATH(rast.at(10, 0), "");

      ASSERT_DEATH(rast(10, 7), "");
      ASSERT_DEATH(rast(0, 7), "");
      ASSERT_DEATH(rast(10, 0), "");
   }

   TEST(RasterNew, GetMinValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMinValue(), 1906.551147);
   }

   TEST(RasterNew, GetMaxValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMaxValue(), 1912.436157);
   }

   TEST(RasterNew, GetMeanValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMeanValue(), 1909.7339340000005);
   }

   TEST(RasterNew, GetNDataPoints)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_EQ(dem.GetNDataPoints(), 44);
   }

   TEST(RasterNew, GetNNoDataPoints)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_EQ(dem.GetNNoDataPoints(), 5);
   }

   TEST(RasterNew, GetValidIndices)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      std::vector<size_t> expected{0,  1,  2,  3,  4,  5,  7,  8,  9,  11, 12, 13, 14, 15, 16,
                                   17, 18, 19, 20, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                                   34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48};

      auto out = dem.GetValidIndices();

      ASSERT_EQ(expected, out);
   }

} // namespace KiLib
