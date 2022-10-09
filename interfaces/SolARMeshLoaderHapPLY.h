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

#ifndef SOLARMESHLOADERHAPPLY_H
#define SOLARMESHLOADERHAPPLY_H

#include <xpcf/component/ComponentBase.h>
#include <xpcf/component/ConfigurableBase.h>
#include <api/input/files/IMeshLoader.h>

#include <happly.h>

/**
 * @class SolARMeshLoaderHapPLY
 * @brief <B>Load a ply file into a SolAR mesh</B>
 * <TT>UUID: 394c2efc-4647-11ed-b878-0242ac120002</TT>
 *
 * @SolARComponentPropertiesBegin
 * @SolARComponentProperty{ path,
 *                          the path and filename of the ply file to load,
 *                          @SolARComponentPropertyDescString{ "mesh.ply" }}
 * @SolARComponentPropertiesEnd
 *
 */

namespace SolAR::MODULES::hapPLY
{

class SolARMeshLoaderHapPLY : public org::bcom::xpcf::ConfigurableBase, virtual public api::input::files::IMeshLoader
{

public:

    SolARMeshLoaderHapPLY();
    ~SolARMeshLoaderHapPLY() override = default;
    void unloadComponent () override final;

    /// @brief Load a Mesh from a file
    /// @param[out] Mesh: the point cloud to fill
    /// @return FrameworkReturnCode::_SUCCESS if load succeed, else FrameworkReturnCode::_ERROR_
    FrameworkReturnCode load(SRef<SolAR::datastructure::Mesh> & Mesh) override;

private:
    std::string m_path = "mesh.ply";

    std::vector<std::array<double, 3>> getVertexNormals (happly::PLYData& plyData, const std::string& vertexElementName = "vertex");
};

} // namespace SolAR::MODULES::hapPLY



template <> struct org::bcom::xpcf::ComponentTraits<SolAR::MODULES::hapPLY::SolARMeshLoaderHapPLY>
{
    static constexpr const char * UUID = "{394c2efc-4647-11ed-b878-0242ac120002}";
    static constexpr const char * NAME = "SolARMeshLoaderHapPLY";
    static constexpr const char * DESCRIPTION = "SolARMeshLoaderHapPLY implements api::output::files::IMeshLoader interface";
};

#endif // SOLARMESHLOADERHAPPLY_H
