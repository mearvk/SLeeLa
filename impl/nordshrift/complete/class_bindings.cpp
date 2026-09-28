#include "class_bindings.h"
#include <filesystem>

namespace nordshrift::complete {
static const std::vector<ClassBinding> kBindings = {
{"designer","ProjectSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"designer","ModuleSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"designer","ClassSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"designer","InterfaceSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"designer","MethodSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"designer","FieldSpecification","api/sleela-complete/DesignerClasses.sleela"},
{"specification","ParameterSpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"specification","TypeSpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"specification","EnumSpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"specification","EventSpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"specification","DependencySpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"specification","CapabilitySpecification","api/sleela-complete/SpecificationClasses.sleela"},
{"platform","PlatformSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"platform","BackendSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"platform","AdapterSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"platform","ConnectorSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"platform","BuildSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"platform","TestSpecification","api/sleela-complete/PlatformClasses.sleela"},
{"network-security","NetworkEndpoint","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"network-security","UdpTransport","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"network-security","TcpTransport","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"network-security","DnsResolver","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"network-security","SecureChannel","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"network-security","Credential","api/sleela-complete/NetworkSecurityClasses.sleela"},
{"data-service","DataSet","api/sleela-complete/DataServiceClasses.sleela"},
{"data-service","DatabaseConnection","api/sleela-complete/DataServiceClasses.sleela"},
{"data-service","Query","api/sleela-complete/DataServiceClasses.sleela"},
{"data-service","EmailService","api/sleela-complete/DataServiceClasses.sleela"},
{"data-service","HttpService","api/sleela-complete/DataServiceClasses.sleela"},
{"data-service","ServiceSpecification","api/sleela-complete/DataServiceClasses.sleela"},
{"server-ui-io","Server","api/sleela-complete/ServerUIIOClasses.sleela"},
{"server-ui-io","Router","api/sleela-complete/ServerUIIOClasses.sleela"},
{"server-ui-io","Listener","api/sleela-complete/ServerUIIOClasses.sleela"},
{"server-ui-io","Window","api/sleela-complete/ServerUIIOClasses.sleela"},
{"server-ui-io","Widget","api/sleela-complete/ServerUIIOClasses.sleela"},
{"server-ui-io","File","api/sleela-complete/ServerUIIOClasses.sleela"},
{"runtime","Stream","api/sleela-complete/RuntimeClasses.sleela"},
{"runtime","Thread","api/sleela-complete/RuntimeClasses.sleela"},
{"runtime","Timer","api/sleela-complete/RuntimeClasses.sleela"},
{"runtime","Lock","api/sleela-complete/RuntimeClasses.sleela"},
{"runtime","EventBus","api/sleela-complete/RuntimeClasses.sleela"},
{"runtime","Result","api/sleela-complete/RuntimeClasses.sleela"},
{"domain","Model","api/sleela-complete/DomainClasses.sleela"},
{"domain","Inference","api/sleela-complete/DomainClasses.sleela"},
{"domain","XmlDocument","api/sleela-complete/DomainClasses.sleela"},
{"domain","XmlElement","api/sleela-complete/DomainClasses.sleela"},
{"domain","Subject","api/sleela-complete/DomainClasses.sleela"},
{"domain","KnowledgeUnit","api/sleela-complete/DomainClasses.sleela"},
{"telephony","Call","api/sleela-complete/TelephonyClasses.sleela"},
{"telephony","SipTransaction","api/sleela-complete/TelephonyClasses.sleela"},
{"telephony","RtpStream","api/sleela-complete/TelephonyClasses.sleela"},
{"telephony","MediaStream","api/sleela-complete/TelephonyClasses.sleela"},
{"telephony","SleelaCompleteModule","api/sleela-complete/TelephonyClasses.sleela"},
{"telephony","ModuleSpecificationBuilder","api/sleela-complete/TelephonyClasses.sleela"}
};
const std::vector<ClassBinding>& classBindings(){return kBindings;}
const ClassBinding* findClass(const std::string& m,const std::string& c){for(const auto& b:kBindings)if(m==b.module&&c==b.className)return &b;return nullptr;}
bool canonicalPathExists(const ClassBinding& b,const std::string& root){return std::filesystem::exists(std::filesystem::path(root)/b.canonicalPath);}
std::size_t moduleCount(){std::vector<std::string> n;for(const auto& b:kBindings){bool s=false;for(const auto& x:n)if(x==b.module)s=true;if(!s)n.emplace_back(b.module);}return n.size();}
std::size_t classCount(){return kBindings.size();}
}
