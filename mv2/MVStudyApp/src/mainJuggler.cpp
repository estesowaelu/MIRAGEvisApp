
//----------------------------------------
// Simple OpenGL sample application
//
// main.cpp
//----------------------------------------

#include "JugglerInterface.h"

// --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include "MVStudyApp.h"
#include "SoundManager.h"
using namespace vrj;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	bool isClusterMaster;
	bool isClusterSlave;
	::SoundManager::instance()->playSound("data/startup.wav", 0.5);
//	while(1);
	// Get the kernel
	vrj::Kernel* kernel = vrj::Kernel::instance();
	
	// Instantiate an instance of the app
	JugglerInterface* application = new JugglerInterface(kernel, argc, argv);
	
	// Find the master and slave nodes
	// Create options for the application
	po::options_description asds_desc("Paint Trainer Options");
	po::options_description& general_desc = kernel->getGeneralOptions();
	po::options_description& cluster_desc = kernel->getClusterOptions();
	asds_desc.add(general_desc).add(cluster_desc);
	
	asds_desc.add_options()
	("jconf", po::value< std::vector<std::string> >()->composing(), "VR Juggler Configuration File");
	
	// jconf files can be given as position arguments.
	po::positional_options_description pos_desc;
	pos_desc.add("jconf", -1);
	
	// Construct a parser and do the actual parsing.
	po::command_line_parser parser(argc, argv);
	po::parsed_options parsed = parser.options(asds_desc).positional(pos_desc).allow_unregistered().run();
	
	// Finally store our options and use them.
	po::variables_map vm;
	po::store(parsed, vm);
	po::notify(vm);
	
	if (vm.count("help"))
	{
		std::cout << asds_desc << std::endl;
		delete application;
		return 0;
	}
	
	if ( argc <= 1 )
	{
		// display some usage info (holding the user by the hand stuff)
		//  this will probably go away once the kernel becomes separate
		//  and can load application plugins.
		std::cout<<"\n"<<std::flush;
		std::cout<<"\n"<<std::flush;
		
		std::cout << "Usage: " << argv[0]
		<< " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
		<< std::endl << std::endl;
		
		std::exit(EXIT_FAILURE);
	}
	
	kernel->init(argc, argv);
	
	// Load any config files specified on the command line
	for ( int i = 1; i < argc; ++i )
	{
   		std::cout << "Loading config file named " << argv[i];
		kernel->loadConfigFile(argv[i]);
	}
	// Tell our application if we're a cluster slave and/or master
	isClusterMaster = vm["vrjmaster"].as<bool>();
	isClusterSlave = vm["vrjslave"].as<bool>();
	//LMInitNetwork(isClusterMaster);
	MVStudyApp::instance().setIsMaster(!isClusterSlave);
	//KenTCPTalker::instance().setIsMaster(!isClusterSlave);
	
	// Set the AppController to be master if needed

	kernel->start();
	
	kernel->setApplication(application);
	kernel->waitForKernelStop();
	
	delete application;
	
	return EXIT_SUCCESS;
}
