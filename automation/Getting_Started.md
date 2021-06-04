## Getting Started with FHE on Hyper Protect Virtual Servers

The following steps walk you through how to get a pre-built fhe-toolkit docker image into a Hyper Protect Server container.

### Prerequisites

* You can use any Linux or Unix based (Mac will work) computer that the ibm cloud supports

* If the deploy script will be run on MacOS:
     * Requires a `python` installation, python 3.6+
     * Install `gpg`
          * MacOS with homebrew: `brew install gpg`
               * To install homebrew: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
          * MasOS without homebrew: download from https://gpgtools.org/

     * Install `pyenv`
          * MacOS with homebrew: `brew install pyenv`
          * MacOS without homebrew: `curl https://pyenv.run | bash`
          
* Otherwise, install these dependencies with your Linux distro's package manager

### Creating an IBM Cloud account
* Create an IBM Cloud account if you have never done so. To do this, go to [https://cloud.ibm.com/login](https://cloud.ibm.com/login) in your web browser. 

 If you don't have an account, select the "Create an account" link. [https://cloud.ibm.com/registration](https://cloud.ibm.com/registration)

* Once you have made an account or logged into your existing account,  you must then make an API Key in your IBM Cloud account 

     * At the top of the screen navigate the menus from Manage  > Access (IAM). From the screen shown select the *My IBM Cloud API keys* tile. .  
     
	 * Click Create an IBM Cloud API key.  
     
	 * Enter a helpful a name, and useful description so when you look back at it 6 months from now it'll help you remember what its for.  
     
	 * Click Create.   
     
	 * Click Show.  Once you have that key, copy and save it somewhere that you won't forget or delete it.  Alternatively you can download it as a JSON file from the prompt on screen. 

        * An example API key looks like this `k-y2lsNxfeKbaidcCG6kQVypLDR_WjmDPe-2Uuq_PTYx`



* Update or install `ibmcloud` CLI on the machine you will be using. 

 		[https://cloud.ibm.com/docs/cli?topic=cli-getting-started](https://cloud.ibm.com/docs/cli?topic=cli-getting-started)
 		
 		NOTE: If you have a mac, you can just open a terminal and issue the following command: 
        curl -sL https://raw.githubusercontent.com/IBM-Cloud/ibm-cloud-developer-tools/master/linux-installer/idt-installer | bash

To check this step is complete, you should now have an `ibmcloud cli` command available in your shell.


* Login to ibmcloud `ibmcloud login --apikey <USE_API_KEY>`


Example output follows. If you are logging into the console for the first time you will need to select the region closest to you for best cloud performance. 

```
$ ibmcloud login --apikey *********************************************************************
API endpoint: https://cloud.ibm.com
Authenticating...
OK

Targeted account (10566a6bd5a545b288d656709c528802)


Select a region (or press enter to skip):
1. au-syd
2. in-che
3. jp-osa
4. jp-tok
5. kr-seo
6. eu-de
7. eu-gb
8. ca-tor
9. us-south
10. us-east
Enter a number> 10
Targeted region us-east

                      
API endpoint:      https://cloud.ibm.com   
Region:            us-east   
User:              XXXXXXX@gmail.com   
Account:           XXXX Account (**************************************************)
Resource group:    No resource group targeted, use 'ibmcloud target -g RESOURCE_GROUP'   
CF API endpoint:      
Org:                  
Space:                
```
	
* Setup a namespace:
	
	`ibmcloud cr namespace-add <my_namespace>`
	
 ```
    $ ibmcloud cr namespace-add fhe-hpvs-demo
    No resource group is targeted. Therefore, the default resource group for the account ('Default') is targeted.

    Adding namespace 'fhe-hpvs-demo' in resource group 'Default' for account Eli Dow's Account in registry us.icr.io...

    Successfully added namespace 'fhe-hpvs-demo'

    OK
```

	
* Update to the latest container plugin
	
	`ibmcloud plugin update container-registry`
	
* Install the hpvs plugin as well
	
	`ibmcloud plugin install hpvs`
	
* Create a Resource Group in your account 
    
    my resource group is called : `super_secret`
    
    Login to cloud.ibm.com -> `Manage -> Account -> Account Resources -> Resource Groups -> Create`
    
#### The Keys
To sign and deploy an image to HPVS, we need three to four sets of keys:
1. Root key; root of content trust for an image tag
2. Repository key (aka "target key"); allows you to sign image tags and manage delegations (including delegated keys or permitted delegation paths)
3. Encryption key for the HPVS registration definition file
4. Vendor key, required for signing the HPVS registration definition file
5. Delegation key (optional); allows you to delegate signing image tags to other publishers without having to share your repository key
 
Before you can run `DeployToHPVS.sh`, you must tell it:
1. The passphrase to use for the root key
2. The passphrase to use for the repository key
3. The name and location of the GPG vendor key (both public and private)
4. The name and location of the delegation key (optional)

##### Automatic key generation
If `DeployToHPVS.sh` called with the `-c` option, the interactive wizard will walk you through the process of key generation.

The root and repository keys are generated automatically by `docker trust`, which is invoked by `DeployToHPVS.sh`.  The HPVS registration encryption key is hardcoded into `DeployToHPVS.sh`.  The other keys (the vendor and delegation keys) the interactive wizard will prompt if you want to generate them.  If you want to do it manually, follow the next two steps.  If you want the interactive wizard to do it for you, skip ahead to "Running the HPVS Deploy script" section.

##### Vendor key

To generate a vendor keypair, create a GPG batch file with the following contents:
```
%echo Generating registration definition key
Key-Type: RSA
Key-Length: 4096
Subkey-Type: RSA
Subkey-Length: 4096
Name-Real: fhe_user
Expire-Date: 0
Passphrase: <passphrase>
%echo done
```
Then run the following to generate a new keypair as the `fhe_user` identity:
```
gpg -a --batch --generate-key <batchfile>
```

##### Delegation keys
Delegations in Docker Content Trust (DCT) allow you to control who can and cannot sign an image tag. A delegation will have a pair of private and public delegation keys. A delegation could contain multiple pairs of keys and contributors in order to allow multiple users to be part of a delegation, and to support key rotation.

If `delegationkey` is `false` or commented-out in your configuration file, then the repository key will be used to sign your image. Thereafter, any changes to your image can only be made using the repository key.

Delegation keys can be generated using `docker trust`:
```
docker trust key generate <yourkeyname>
```


### Running the HPVS Deploy script
 * Clone the fhe-toolkit repo `https://github.com/IBM/fhe-toolkit-linux.git`

 * cd into the main folder `cd fhe-toolkit-linux` and build a local image `./BuildDockerImage ubuntu` If you want to test with fedora change `ubuntu` to `fedora` in the command

 * cd into the automation folder `cd fhe-toolkit-linux/automation`

 * Run the HPVS script.  The first time running the script, you will most likely not have any keys or settings setup so you can use the wizard.


 	    `./DeployToHPVS.sh -c [name of config file (to save)] [name of linux image ubuntu or fedora]`
 	    
 * NOTE: Once you have a saved configure file, you can then re-run it using your file like this
 
      `./DeployToHPVS.sh -f DeployToHPVS_configure_file.conf (this is the name of my file)` 
      
Everything should run from here.  Please note where the script gets stuck and errors out (if it does)

### Accessing the toolkit
 When the deploy script has run successfully...
 
 * Log on to the Dashboard section of https://cloud.ibm.com. 
 
 * Expand services in the resource list, click on the image and then click on "View Full Details" to access the public IP address, and copy it to your browser adding the port 8443. It will look like: https://xxx.xx.xx.xx:8443/

 * Acknowledge that the server is using a self-signed certificate, accept risk and continue 

### Running the toolkit
 * Once the toolkit is loaded, you should automatically be prompted to select a kit for the FHE-Workspace to use. Select one of the kits from the dropdown. Configuration of the workspace will begin which you will be able to see in the Output Window. You'll also notice the kit you selected is now shown in the CMake Tools status bar at the bottom of the window.

 * If you are not automatically prompted to select a kit after a few seconds of loading the toolkit, try refreshing your browser. If you're still not prompted, check the CMake Tools status bar at the bottom of the window to see if a kit has already been selected. You'll need to select a kit each time you start a new instance of the Toolkit.

 * Click “Build” in the CMake Tools status bar to build the selected target.

 * When the build has finished, click “Launch” in CMake Tools status bar to launch the selected target in the terminal window.

 Each demonstration application is in a self-contained directory in the examples folder in the IDE. Each demo application directory contains a README.md that explains how to run the demo and what you should expect for results. For instance, the BGV World Country Database Lookup Example Documentation contains the information to run a complete example of a privacy preserving search against an encrypted database. The database is a key value store prepopulated with the english names of countries and their capital cities from the world. Selecting the country will perform a search of the matching capital.



### Debug Mode

To generate more info like a verbose/debug mode when the `./DeployToHPVS` script is running add this var to your command line

    export DEPLOY_TO_HPVS_DEBUG=1
       
 
### How to Delete Everything and start over again
 
 If you have a virtual server app already running in the ibm cloud, you can remove it.  Depending on what type of account that you are subscribed to, you might need to remove it, as only one app is allowed at a time with the free account.
 
 - Log into `cloud.ibm.com` and then go to `Dashboard -> Services`.  if anything is running, or there click on `Actions`, then `Delete Service`.

 - Then, reclaim the resource in the ibmcloud cli (command line interface)
      
        ibmcloud resource reclamations

Find the reclamation id of the running resource and use that in the next command to actually delete the resource

        ibmcloud resource reclamation-delete RECLAMATION_ID


 - Delete the local image too, if no longer needed

    docker image rmi -f [docker image id in this example it was 86c15e968f8b]
    
When a new image is signed, there are a few keys that are created and become associated with that image.  If you wish to push that image to the virtual server again, you will need those keys to sign and verify.  This is the key pair in the config file called the delegation key.  It is signed using a repo key that is stored in `~/.docker/trust/private`.  The name of the key is a random string that is generated.  You will need to hang onto this key to re-sign this image and any others associated with it.  You can remove the delegation key if you no longer need it, but leave the repo key unless you are going to create a new one.
    
 - delete the docker trust info

      notary delete us.icr.io/super_secure/fhe-toolkit-ubuntu-s390x --remote -s https://us.icr.io:4443
      
- then delete it locally

     notary delete us.icr.io/super_secure/fhe-toolkit-ubuntu-s390x

After these steps are complete, the script should re-run successfully, but if an error is thrown that says something akin to "Can't Find the key" and its looking for a specific id you will need to remove the existing signer

- Remove any existing signer

     DOCKER_CONTENT_TRUST_SERVER=https://us.icr.io:4443 docker trust signer remove fhe_sample_delegation  us.icr.io/super_secure/fhe-toolkit-fedora-s390x

After these steps are done, it should be cleared out and you can re-run the script


 
 
