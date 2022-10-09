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

#include <api/input/files/IMeshLoader.h>
#include <api/output/files/IMeshExporter.h>

using namespace std;

using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;

namespace xpcf = org::bcom::xpcf;

int main(int argc, char *argv[])
{

#if NDEBUG
    boost::log::core::get()->set_logging_enabled(false);
#endif

    LOG_ADD_LOG_TO_CONSOLE();

	SRef<xpcf::IComponentManager> xpcfComponentManager = xpcf::getComponentManagerInstance();

    string configxml = "SolARTest_ModuleHapPLY_MeshIO_conf.xml";
	if (xpcfComponentManager->load(configxml.c_str()) != org::bcom::xpcf::_SUCCESS)
	{
		LOG_ERROR("Failed to load the configuration file {}", configxml.c_str());
		return -1;
	}

	LOG_INFO("Start creating components");
    auto meshLoader = xpcfComponentManager->resolve<input::files::IMeshLoader>();
    auto meshExporter = xpcfComponentManager->resolve<SolAR::api::output::files::IMeshExporter>();
	LOG_INFO("Components created!");

    SRef<Mesh> mesh;

    LOG_INFO("Loading the mesh '{}'", meshLoader->bindTo<xpcf::IConfigurable>()->getProperty("path")->getStringValue());
    if (meshLoader->load(mesh) == FrameworkReturnCode::_SUCCESS)
    {
        LOG_INFO("Mesh loaded!");
	}
    else
    {
        LOG_ERROR("Failed to load the mesh!");
		return -1;
	}

    if ( meshExporter->exportMesh(mesh) != FrameworkReturnCode::_SUCCESS)
	{
        LOG_ERROR("Failed to export the mesh!");
		return -1;
	}

    LOG_INFO("mesh exported to '{}'", meshExporter->bindTo<xpcf::IConfigurable>()->getProperty("path")->getStringValue());

    return 0;
}
