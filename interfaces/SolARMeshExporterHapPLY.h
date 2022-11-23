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

#ifndef SOLARMESHEXPORTERHAPPLY_H
#define SOLARMESHEXPORTERHAPPLY_H

#include <xpcf/component/ComponentBase.h>
#include <xpcf/component/ConfigurableBase.h>

#include <api/output/files/IMeshExporter.h>

#include <happly.h>

/**
 * @class SolARMeshExporterHapPLY
 * @brief <B>Export a SolAR mesh into a ply file</B>
 * <TT>UUID: 331ee220-4630-11ed-b878-0242ac120002</TT>
 *
 * @SolARComponentPropertiesBegin
 * @SolARComponentProperty{ dataFormat,
 *                          the output format of the ply (either Binary, ASCII, or BinaryBigEndian)
 *                          @SolARComponentPropertyDescString{ "Binary" }}
 * @SolARComponentProperty{ path,
 *                          the path and filename where the mesh will be export,
 *                          @SolARComponentPropertyDescString{ "mesh.ply" }}
 * @SolARComponentPropertiesEnd
 *
 */

namespace SolAR::MODULES::hapPLY
{

class SolARMeshExporterHapPLY : public org::bcom::xpcf::ConfigurableBase, virtual public api::output::files::IMeshExporter
{

public:
    std::map<std::string, happly::DataFormat> m_stringToHapplyDataFormat =
    {
        {"ASCII", happly::DataFormat::ASCII},
        {"Binary", happly::DataFormat::Binary},
        {"BinaryBigEndian", happly::DataFormat::BinaryBigEndian},
    };

    SolARMeshExporterHapPLY();
    ~SolARMeshExporterHapPLY() = default;
    void unloadComponent () override final;

    FrameworkReturnCode exportMesh(const SRef<SolAR::datastructure::Mesh> & Mesh) override;

private:
    std::string m_dataFormat = "Binary";
    std::string m_path = "mesh.ply";

    void addVertexNormals(happly::PLYData& plyData, std::vector<std::array<double, 3>>& vertexNormals);
};

} // namespace SolAR::MODULES::hapPLY

XPCF_DEFINE_COMPONENT_TRAITS(SolAR::MODULES::hapPLY::SolARMeshExporterHapPLY,
                             "331ee220-4630-11ed-b878-0242ac120002",
                             "SolARMeshExporterHapPLY",
                             "SolARMeshExporterHapPLY implements api::output::files::IMeshExporter interface")

#endif // SOLARMESHEXPORTERHAPPLY_H
