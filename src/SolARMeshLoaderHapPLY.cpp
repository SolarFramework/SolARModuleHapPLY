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

#include "SolARMeshLoaderHapPLY.h"
#include <numeric>

#include <core/Log.h>

namespace xpcf = org::bcom::xpcf;

XPCF_DEFINE_FACTORY_CREATE_INSTANCE(SolAR::MODULES::hapPLY::SolARMeshLoaderHapPLY)

namespace SolAR::MODULES::hapPLY
{

SolARMeshLoaderHapPLY::SolARMeshLoaderHapPLY():xpcf::ConfigurableBase(xpcf::toMap<SolARMeshLoaderHapPLY>())
{
    declareInterface<api::input::files::IMeshLoader>(this);
    declareProperty<std::string>("path" , m_path);
    LOG_DEBUG("SolARMeshLoaderHapPLY constructor");
}

FrameworkReturnCode SolARMeshLoaderHapPLY::load(SRef<SolAR::datastructure::Mesh> & mesh)
{
    try
    {
        if (mesh == nullptr)
            mesh = xpcf::utils::make_shared<SolAR::datastructure::Mesh>();
        happly::PLYData plyIn(m_path);

        std::vector<SolAR::datastructure::Point3Df>  points;
        std::vector<SolAR::datastructure::Vector3f>  colors;
        std::vector<SolAR::datastructure::Vector3ui> ifs;
        std::vector<SolAR::datastructure::Vector3f>  normals;

        std::vector<std::array<double, 3>>        meshVertexPositions = plyIn.getVertexPositions();
        std::vector<std::array<unsigned char, 3>> meshVertexColors = plyIn.getVertexColors();
        std::vector<std::array<double, 3>>        meshVertexNormals = getVertexNormals(plyIn);
        std::vector<std::vector<unsigned int>>    meshFaceIndices =  plyIn.getFaceIndices<unsigned int>();


        for( const auto& p : meshVertexPositions )
        {
            points.push_back(datastructure::Point3Df(p[0], p[1], p[2]));
        }

        for( const auto& c : meshVertexColors )
        {
            colors.push_back(datastructure::Vector3f({ c[0]/255.0f, c[1]/255.0f, c[2]/255.0f}));
        }

        for( const auto& faceIndicies : meshFaceIndices )
        {
            ifs.push_back(datastructure::Vector3ui({ faceIndicies[0], faceIndicies[1], faceIndicies[2]}));
        }

        for( const auto& normal : meshVertexNormals )
        {
            normals.push_back(datastructure::Vector3f({(float)normal[0],(float)normal[1],(float)normal[2]}));
        }

        mesh->setPoints(points);
        mesh->setColors(colors);
        mesh->setIndexedFaceSets(ifs);

        return FrameworkReturnCode::_SUCCESS;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Failed to load ply mesh: {}", e.what());
        return FrameworkReturnCode::_ERROR_;
    }
}

std::vector<std::array<double, 3>> SolARMeshLoaderHapPLY::getVertexNormals (happly::PLYData& plyData, const std::string& vertexElementName)
{
    if (!plyData.getElement(vertexElementName).hasProperty("nx") ||
        !plyData.getElement(vertexElementName).hasProperty("ny") ||
        !plyData.getElement(vertexElementName).hasProperty("nz"))
        return std::vector<std::array<double, 3>>();

    std::vector<double> nx = plyData.getElement(vertexElementName).getProperty<double>("nx");
    std::vector<double> ny = plyData.getElement(vertexElementName).getProperty<double>("ny");
    std::vector<double> nz = plyData.getElement(vertexElementName).getProperty<double>("nz");

    std::vector<std::array<double, 3>> result(nx.size());
    for (size_t i = 0; i < result.size(); i++)
    {
      result[i][0] = nx[i];
      result[i][1] = ny[i];
      result[i][2] = nz[i];
    }

    return result;
}

} // namespace SolAR:MODULES::hapPLY
