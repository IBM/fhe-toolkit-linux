## Getting Started with FHE on Hyper Protect Virtual Servers

The following is how to get a pre-built fhe-toolkit docker image into a Hyper Protect Server container.

###Preparing my system to handle a HPVS connection

* Create an IBM Cloud account `cloud.ibm.com`

* Make an API Key in your ibm cloud account 

     * Go to Manage > Access (IAM) > API keys.  
	 * Click Create an IBM Cloud API key.  
	 * Enter a helpful a name, and useful description so when you look back at it 6 months from now it'll help you remember what its for. 
	 * Click Create.   
	 * Click Show.  Once you have that key, copy and save it somewhere that you won't forget or delete it.  

* Update or install `ibmcloud` CLI on local machine & on the S390 machine you will be using.  Its good to have  it working on another non-S390x machine because there are some things that you can't do 
 		[https://cloud.ibm.com/docs/Registry?topic=Registry-getting-started](https://cloud.ibm.com/docs/Registry?topic=Registry-getting-started)

* Login to ibmcloud `ibmcloud login --apikey <USE_API_KEY>`
	
* Setup a namespace:
	
	`ibmcloud cr namespace-add <my_namespace>`
	
* Update to the latest container plugin (not sure this is available on S390)
	
	`ibmcloud plugin update container-registry`
	
* Also install the hpvs plugin as well (this is definitely NOT on S390)
	
	`ibmcloud plugin install hpvs`
	
* Create a Resource Group in your account 
    
    my resource group is : `super_secret`
    
    Login to cloud.ibm.com -> `Manage -> Account -> Account Resources -> Resource Groups -> Create`
    
 * Login onto the S390x machine (see greg for credentials if you dont have them)

 * Clone the fhe-toolkit repo `https://github.com/IBM/fhe-toolkit-linux.git`

 * Checkout the hpvs_work_integration branch `git checkout hpvs_work_integration`

 * cd into the main folder `cd fhe-toolkit-linux` and build a local image `./BuildDockerImage ubuntu` If you want to test with fedora change `ubuntu` to `fedora` in the command

 * cd into the automation folder `cd fhe-toolkit-linux/automation`

 * Run the HPVS script.  The first time running the script, you will most likely not have any keys or settings setup so you can use the wizard.


 	    `./DeployToHPVS.sh -l ubuntu -c [name of config file (to save)]`
 	    
 Once you have a saved configure file, you can then re-run it using your file like this
 
      `./DeployToHPVS.sh -l ubuntu -f DeployToHPVS.conf.boland(this is the name of my file)` 
      
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


 
 