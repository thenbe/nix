#ifndef __DERIVATIONS_H
#define __DERIVATIONS_H

typedef struct _ATerm * ATerm;

#include "hash.hh"
#include "util.hh"

#include <map>


namespace nix {


/* Extension of derivations in the Nix store. */
const string drvExtension = ".drv";


/* Abstract syntax of derivations. */

struct DerivationOutput
{
    Path path;
    string hashAlgo; /* hash used for expected hash computation */
    string hash; /* expected hash, may be null */
    DerivationOutput()
    {
    }
    DerivationOutput(Path path, string hashAlgo, string hash)
    {
        this->path = path;
        this->hashAlgo = hashAlgo;
        this->hash = hash;
    }
};

struct DerivationStateOutput
{
    Path statepath;
    string hashAlgo;
    string hash;
    string stateIdentifier;				//the identifier
    string enabled;						//enable or disable state
    string shared;						//none, full, group
    string synchronization;				//none (no locks), exclusive-lock-on-own-state-dir, exclusive-lock-on-all-(sub)-states-dir
    
    string commitReferences;			//TODO none, direct, recursive-all
    string commitBinaries;				//TODO list of binaries that need (or not) to be committed when these binaries are called
         
    string createDirsBeforeInstall;		//if true: creates state dirs before installation
	string runtimeStateParamters;		//if not empty: these are the runtime parameters where state can be found (you can use $statepath here)
    DerivationStateOutput()
    {
    }
    DerivationStateOutput(Path statepath, string hashAlgo, string hash, string stateIdentifier, string enabled, string shared, string synchronization, string createDirsBeforeInstall, string runtimeStateParamters)
    {
        this->statepath = statepath;
        this->hashAlgo = hashAlgo;
        this->hash = hash;
        this->stateIdentifier = stateIdentifier;
        this->enabled = enabled;
        this->shared = shared;
        this->synchronization = synchronization;
        this->createDirsBeforeInstall = createDirsBeforeInstall;
        this->runtimeStateParamters = runtimeStateParamters;
    }
    
    bool getEnabled(){
    	return string2bool(enabled);
    }
    
    bool getCreateDirsBeforeInstall(){
    	return string2bool(createDirsBeforeInstall);
    }
};

struct DerivationStateOutputDir
{
    string path;
    string type;					//none, manual, interval, full
    string interval;				//type int
    DerivationStateOutputDir()
    {
    }
    DerivationStateOutputDir(string path, string type, string interval)
    {
        this->path = path;
        this->type = type;
        this->interval = interval;
    }
    
    int getInterval(){
    	return 0;			//TODO
    }
    
    //sort function
	bool operator<(const DerivationStateOutputDir& a) const { return path < a.path; }      
};


typedef std::map<string, DerivationOutput> DerivationOutputs;
typedef std::map<string, DerivationStateOutput> DerivationStateOutputs;
typedef std::map<string, DerivationStateOutputDir> DerivationStateOutputDirs;


/* For inputs that are sub-derivations, we specify exactly which
   output IDs we are interested in. */
typedef std::map<Path, StringSet> DerivationInputs;
typedef std::map<string, string> StringPairs;

struct Derivation
{
    DerivationOutputs outputs; /* keyed on symbolic IDs */
    DerivationStateOutputs stateOutputs; /*  */
    DerivationStateOutputDirs stateOutputDirs; /*  */    
    DerivationInputs inputDrvs; /* inputs that are sub-derivations */
    PathSet inputSrcs; /* inputs that are sources */
    string platform;
    Path builder;
    Strings args;
    StringPairs env;
};


/* Hash an aterm. */
Hash hashTerm(ATerm t);

/* Write a derivation to the Nix store, and return its path. */
Path writeDerivation(const Derivation & drv, const string & name);

/* Parse a derivation. */
Derivation parseDerivation(ATerm t);

/* Parse a derivation. */
ATerm unparseDerivation(const Derivation & drv);

/* Check whether a file name ends with the extensions for
   derivations. */
bool isDerivation(const string & fileName);

 
}


#endif /* !__DERIVATIONS_H */
