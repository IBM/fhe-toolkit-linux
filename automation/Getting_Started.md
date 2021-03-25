## Getting Started with FHE on Hyper Protect Virtual Servers

The following is how to get a pre-built fhe-toolkit docker image into a Hyper Protect Server container.

###Preparing my system to handle a HPVS connection

* Create an IBM Cloud account if you have never done so. To do this, go to [https://cloud.ibm.com/login](https://cloud.ibm.com/login) in your web browser. 

 If you don't have an account, select the "Create an account" link. [https://cloud.ibm.com/registration](https://cloud.ibm.com/registration)

* Once you have made an account or logged into your existing account,  you must then make an API Key in your ibm cloud account 

     * At the top of the screen navigate the menus from Manage  > Access (IAM). From the screen shown select the *My IBM Cloud API keys* tile. .  
     
	 * Click Create an IBM Cloud API key.  
     
	 * Enter a helpful a name, and useful description so when you look back at it 6 months from now it'll help you remember what its for.  
     
	 * Click Create.   
     
	 * Click Show.  Once you have that key, copy and save it somewhere that you won't forget or delete it.  Alternatively you can download it as a JSON file from the prompt on screen. 

* Update or install `ibmcloud` CLI on local machine & on the S390 machine you will be using.  Its good to have  it working on another non-S390x machine because there are some things that you can't do 
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

	
* Update to the latest container plugin (not sure this is available on S390)
	
	`ibmcloud plugin update container-registry`
	
* Also install the hpvs plugin as well (this is definitely NOT on S390)
	
	`ibmcloud plugin install hpvs`
	
* Create a Resource Group in your account 
    
    my resource group is : `super_secret`
    
    Login to cloud.ibm.com -> `Manage -> Account -> Account Resources -> Resource Groups -> Create`
    
* If the deploy script will be run on MacOS:
     * Requires a `python` installation
     * Install `gpg`
          * MacOS with homebrew: `brew install gpg`
               * To install homebrew: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
          * MasOS without homebrew: download from https://gpgtools.org/

     * Install `pyenv`
          * MacOS with homebrew: `brew install pyenv`
          * MacOS without homebrew: `curl https://pyenv.run | bash`

 * Login onto the S390x machine (see greg for credentials if you dont have them)

 * Clone the fhe-toolkit repo `https://github.com/IBM/fhe-toolkit-linux.git`

 * Checkout the hpvs_work_integration branch `git checkout hpvs_work_integration`

 * cd into the main folder `cd fhe-toolkit-linux` and build a local image `./BuildDockerImage ubuntu` If you want to test with fedora change `ubuntu` to `fedora` in the command

 * cd into the automation folder `cd fhe-toolkit-linux/automation`

 * Run the HPVS script.  The first time running the script, you will most likely not have any keys or settings setup so you can use the wizard.


 	    `./DeployToHPVS.sh -c [name of config file (to save)] [name of linux image ubuntu or fedora]`
 	    
 Once you have a saved configure file, you can then re-run it using your file like this
 
      `./DeployToHPVS.sh -f DeployToHPVS_configure_file.conf (this is the name of my file)` 
      
Everything should run from here.  Please note where the script gets stuck and errors out (if it does)


### Debug Mode

To generate more info like a verbose/debug mode when the `./DeployToHPVS` script is running add this var to your command line

    export DEPLOY_TO_HPVS_DEBUG=1
       
 
### How to delete everything that we have and start over again
 
 IF you have a virtual server app already running in the ibm cloud, you can remove it.  Depending on what type of account that you are subscribed to, you might need to remove it, as only 1 app is allowed with the free account.
 
 - Log into `cloud.ibm.com` and then go to `Dashboard -> Services`.  if anything is running, or there click on `Actions`, then `Delete Service`.

 - Then you need to reclaim the resource in the ibmcloud cli (command line interface)
      
        ibmcloud resource reclamations
        ibmcloud resource reclamation-delete RECLAMATION_ID


 - Delete the local image too, cause why not

    docker image rmi -f [docker image id in this example it was 86c15e968f8b]
    
 - delete the docker trust info (this needs to be done on an non-s390 machine, acutally not sure about this one, should be able to use s390) that has the proper ibmcloud cli installed

      notary delete us.icr.io/boland_super_secure/fhe-toolkit-ubuntu-s390x --remote -s https://us.icr.io:4443
      
- then delete it locally (this I think should be done on the s390 machine

     notary delete us.icr.io/boland_super_secure/fhe-toolkit-ubuntu-s390x

Not Sure we need these steps.  After we get this far, we should be able to re-run the script, but if you run into an error that says something like "Can't Find the key" and says its looking for a specific id you will need to remove the existing signer

- Remove any existing signer

     DOCKER_CONTENT_TRUST_SERVER=https://us.icr.io:4443 docker trust signer remove fhe_sample_delegation  us.icr.io/boland_super_secure/fhe-toolkit-fedora-s390x

After these steps are done, it should be cleared out and you can re-run the script


 
 