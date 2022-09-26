/**
 * @copyright Copyright (c) 2022 B-com http://www.b-com.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SolARPointCloudExporterHapPLY.h"

#include <numeric>

#include <core/Log.h>

namespace xpcf = org::bcom::xpcf;

XPCF_DEFINE_FACTORY_CREATE_INSTANCE(SolAR::MODULES::hapPLY::SolARPointCloudExporterHapPLY)

namespace SolAR::MODULES::hapPLY
{

SolARPointCloudExporterHapPLY::SolARPointCloudExporterHapPLY():xpcf::ConfigurableBase(xpcf::toMap<SolARPointCloudExporterHapPLY>())
{
    declareInterface<api::output::files::IPointCloudExporter>(this);
    declareProperty<std::string>("dataFormat" , m_dataFormat);
}

FrameworkReturnCode SolARPointCloudExporterHapPLY::exportPointCloud(const std::string & filepath, const SRef<SolAR::datastructure::PointCloud> & pointCloud)
{
    try
    {
        std::vector<SRef<SolAR::datastructure::CloudPoint>> points;
        pointCloud->getAllPoints(points);

        std::vector<std::array<double, 3>> meshVertexPositions;
        std::vector<std::array<double, 3>> meshVertexColors;
        for( const auto& p : points )
        {
            meshVertexPositions.push_back( { p->x(), p->y(), p->z() } );
            meshVertexColors.push_back( { p->getR(), p->getG(), p->getB() });
        }

        happly::PLYData plyOut;
        plyOut.addVertexPositions(meshVertexPositions);
        plyOut.addVertexColors(meshVertexColors);
        plyOut.write(filepath, m_stringToHapplyDataFormat.at(m_dataFormat));

        return FrameworkReturnCode::_SUCCESS;
    }
    catch( const std::out_of_range& e)
    {
        LOG_ERROR("Failed to export point cloud to PLY file, unknown PLY format requested: '{}' (valid formats are: [{}])",
                  m_dataFormat,
                  std::accumulate(m_stringToHapplyDataFormat.begin(),
                                  m_stringToHapplyDataFormat.end(),
                                  std::string(),
                                  [](const std::string& acc, const std::pair<const std::string, const happly::DataFormat>& elt)
                                  {
                                    return acc.empty() ? elt.first : acc + std::string(", ") + elt.first;
                                  }));

        return FrameworkReturnCode::_ERROR_;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Failed to export point cloud to PLY file: {}", e.what());
        return FrameworkReturnCode::_ERROR_;
    }
}

} // namespace SolAR:MODULES::hapPLY
