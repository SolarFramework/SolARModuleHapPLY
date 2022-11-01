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

#include "SolARMeshExporterHapPLY.h"
#include <numeric>

#include <core/Log.h>

namespace xpcf = org::bcom::xpcf;

XPCF_DEFINE_FACTORY_CREATE_INSTANCE(SolAR::MODULES::hapPLY::SolARMeshExporterHapPLY)

namespace SolAR::MODULES::hapPLY
{

SolARMeshExporterHapPLY::SolARMeshExporterHapPLY():xpcf::ConfigurableBase(xpcf::toMap<SolARMeshExporterHapPLY>())
{
    declareInterface<api::output::files::IMeshExporter>(this);
    declareProperty<std::string>("dataFormat" , m_dataFormat);
    declareProperty<std::string>("path" , m_path);
    LOG_DEBUG("SolARMeshExporterHapPLY constructor");
}

FrameworkReturnCode SolARMeshExporterHapPLY::exportMesh(const SRef<SolAR::datastructure::Mesh> & mesh)
{
    try
    {
        std::vector<SolAR::datastructure::Point3Df>  points = mesh->getPoints();
        std::vector<SolAR::datastructure::Vector3f>  colors = mesh->getColors();
        std::vector<SolAR::datastructure::Vector3f>  normals = mesh->getNormals();
        std::vector<SolAR::datastructure::Vector3ui> ifs = mesh->getIndexedFaceSets();

        std::vector<std::array<double, 3>>      meshVertexPositions;
        std::vector<std::array<double, 3>>      meshVertexColors;
        std::vector<std::array<double, 3>>      meshVertexNormals;
        std::vector<std::vector<unsigned int>>  meshFaceIndices;

        for( const auto& p : points )
        {
            meshVertexPositions.push_back({ p.x(), p.y(), p.z()});

        }

        for( const auto& c : colors )
        {
            meshVertexColors.push_back( { c[0], c[1], c[2]});
        }

        for( const auto& faceIndicies : ifs )
        {
            meshFaceIndices.push_back( { faceIndicies[0], faceIndicies[1], faceIndicies[2]});
        }

        for( const auto& normal : normals )
        {
            meshVertexNormals.push_back( { normal[0], normal[1], normal[2]} );
        }

        happly::PLYData plyOut;
        plyOut.addVertexPositions(meshVertexPositions);
        plyOut.addVertexColors(meshVertexColors);
        addVertexNormals(plyOut, meshVertexNormals);
        plyOut.addFaceIndices(meshFaceIndices);
        plyOut.write(m_path, m_stringToHapplyDataFormat.at(m_dataFormat));

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

void SolARMeshExporterHapPLY::addVertexNormals(happly::PLYData& plyData, std::vector<std::array<double, 3>>& vertexNormals)
{
    std::string vertexName = "vertex";
    size_t N = vertexNormals.size();

    if (N == 0)
        return;

    // Create the element
    if (!plyData.hasElement(vertexName)) {
      plyData.addElement(vertexName, N);
    }

    // De-interleave
    std::vector<double> nx(N);
    std::vector<double> ny(N);
    std::vector<double> nz(N);
    for (size_t i = 0; i < vertexNormals.size(); i++) {
      nx[i] = vertexNormals[i][0];
      ny[i] = vertexNormals[i][1];
      nz[i] = vertexNormals[i][2];
    }

    // Store
    plyData.getElement(vertexName).addProperty<double>("x", nx);
    plyData.getElement(vertexName).addProperty<double>("y", ny);
    plyData.getElement(vertexName).addProperty<double>("z", nz);
}
} // namespace SolAR:MODULES::hapPLY
