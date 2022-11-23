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

#ifndef SOLARPOINTCLOUDEXPORTERHAPPLY_H
#define SOLARPOINTCLOUDEXPORTERHAPPLY_H

#include <xpcf/component/ComponentBase.h>
#include <xpcf/component/ConfigurableBase.h>

#include <api/output/files/IPointCloudExporter.h>

#include <happly.h>

namespace SolAR::MODULES::hapPLY
{

class SolARPointCloudExporterHapPLY : public org::bcom::xpcf::ConfigurableBase, virtual public api::output::files::IPointCloudExporter
{

public:
    std::map<std::string, happly::DataFormat> m_stringToHapplyDataFormat =
    {
        {"ASCII", happly::DataFormat::ASCII},
        {"Binary", happly::DataFormat::Binary},
        {"BinaryBigEndian", happly::DataFormat::BinaryBigEndian},
    };

    SolARPointCloudExporterHapPLY();
    ~SolARPointCloudExporterHapPLY() override = default;
    void unloadComponent () override final;

    FrameworkReturnCode exportPointCloud(const SRef<SolAR::datastructure::PointCloud> & pointCloud) override;

private:
    std::string m_dataFormat = "Binary";
    std::string m_path = "pointcloud.ply";
};

} // namespace SolAR::MODULES::hapPLY

XPCF_DEFINE_COMPONENT_TRAITS(SolAR::MODULES::hapPLY::SolARPointCloudExporterHapPLY,
                             "4b5e0353-bc37-4bab-b0a3-89460e1752c6",
                             "SolARPointCloudExporterHapPLY",
                             "SolARPointCloudExporterHapPLY implements api::output::files::IPointCloudExporter interface")

#endif // SOLARPOINTCLOUDEXPORTERHAPPLY_H
