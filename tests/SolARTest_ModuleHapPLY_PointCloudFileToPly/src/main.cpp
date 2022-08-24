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

#include <boost/log/core.hpp>

#include <xpcf/xpcf.h>

#include <core/Log.h>

#include <api/output/files/IPointCloudExporter.h>
#include <api/storage/IPointCloudManager.h>

using namespace std;

using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;
using namespace SolAR::api::storage;

namespace xpcf = org::bcom::xpcf;

int main(int argc, char *argv[])
{

#if NDEBUG
    boost::log::core::get()->set_logging_enabled(false);
#endif

    LOG_ADD_LOG_TO_CONSOLE();

	SRef<xpcf::IComponentManager> xpcfComponentManager = xpcf::getComponentManagerInstance();

    if( argc != 2 )
    {
        LOG_ERROR("Error: incorrect number of parameters");
        LOG_ERROR("Usage:");
        LOG_ERROR("  {} <path to point cloud .bin file>", argv[0]);
        return -1;
    }

    string poincloudFile = string(argv[1]);
    string outputPlyFile = "pointcloud.ply";

    string configxml = "SolARTest_ModuleHapPLY_PointCloudFileToPly_conf.xml";
	if (xpcfComponentManager->load(configxml.c_str()) != org::bcom::xpcf::_SUCCESS)
	{
		LOG_ERROR("Failed to load the configuration file {}", configxml.c_str());
		return -1;
	}

	LOG_INFO("Start creating components");
	auto pointCloudManager = xpcfComponentManager->resolve<storage::IPointCloudManager>();
    auto pointCloudExporter = xpcfComponentManager->resolve<SolAR::api::output::files::IPointCloudExporter>();
	LOG_INFO("Components created!");

    LOG_INFO("Loading point cloud from '{}'", poincloudFile);
    if (pointCloudManager->loadFromFile(poincloudFile) == FrameworkReturnCode::_SUCCESS)
    {
		LOG_INFO("Point cloud loaded!");
	}
    else
    {
		LOG_ERROR("Failed to load point cloud!");
		return -1;
	}

	const SRef<PointCloud>& pointCloud = pointCloudManager->getConstPointCloud();
    if ( pointCloudExporter->save(outputPlyFile, pointCloud) != FrameworkReturnCode::_SUCCESS)
	{
		LOG_ERROR("Failed to export point cloud!");
		return -1;
	}

    LOG_INFO("Point cloud exported to '{}'", outputPlyFile);

    return 0;
}
