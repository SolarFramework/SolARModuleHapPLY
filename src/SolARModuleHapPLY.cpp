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

#include "SolARModuleHapPLYAPI.h"
#include "SolARPointCloudExporterHapPLY.h"

#include <iostream>

#include <xpcf/module/ModuleFactory.h>


namespace xpcf=org::bcom::xpcf;

XPCF_DECLARE_MODULE("{8a6221ff-8c38-4c72-a655-73a2befc33b6}","SolAR::MODULES::SolARModuleHapPLY","SolARModuleHapPLY module description");

extern "C" XPCF_MODULEHOOKS_API xpcf::XPCFErrorCode XPCF_getComponent(const xpcf::uuids::uuid& componentUUID,SRef<xpcf::IComponentIntrospect>& interfaceRef)
{
    xpcf::XPCFErrorCode errCode = xpcf::XPCFErrorCode::_FAIL;
    errCode = xpcf::tryCreateComponent<SolAR::MODULES::hapPLY::SolARPointCloudExporterHapPLY>(componentUUID,interfaceRef);
    return errCode;
}

XPCF_BEGIN_COMPONENTS_DECLARATION

XPCF_END_COMPONENTS_DECLARATION
