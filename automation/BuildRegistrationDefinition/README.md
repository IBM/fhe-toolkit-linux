Module BuildRegistrationDefinition.__main__
===========================================
Build a Hyper Protect Virtual Server registration definition file

Author(s)
---------

Dan FitzGerald <danfitz@us.ibm.com>  
Lei A.B. Wang <wlwangwl@cn.ibm.com>

Usage
-----
    python3 -m BuildRegistrationDefinition --help
    
### Track diagram ###
  
    >>-- python3 -m BuildRegistrationDefinition --| OPTIONS |--| ARGUMENTS |--><
    
### Options ###

       .------------------------------------------------------------.
       V                                                            |
    >-----+------------------------------------------------------+--+---------><
          |               .-| namespace |-.                      |
          +--- --dockerid + USERID--------+----------------------+
          +--- --dockerpass PASSWORD ----------------------------+
          +-+ --help +-------------------------------------------+
          | '- -h ---'                                           |
          |                 .- $PWD/hpvs-fhe-registration.txt -. |
          +--- --outputfile +- PATH ---------------------------+-+
          '--- --version ----------------------------------------'

#### `--dockerid` ####
 * Userid for a container repository, such as Docker Hub or IBM Cloud
 * If omitted, will assume the same value as `namespace`

#### `--dockerpass` ####
 * Password for container repository
 * If omitted, this program will prompt for the container repository password

#### `--help` ####
 * Display usage information and exit
        
#### `--outputfile` ####
 * Path to write output JSON file
 * If omitted, JSON output will be written to `$PWD/hpvs-fhe-registration.txt`
        
#### `--vendorkeyfile` ####
 * Path of vendor key file that was used to sign the image

#### `--version` ####
 * Show version information and exit
    
### Arguments ###
    
    >-- registry -- namespace -- repository -- path_to_signing_key -----------><
        
#### registry ####
 * Name of target container registry
 * Acceptable values are `hub`, `docker.io`, or the registry's URL

#### namespace ####
 * Target namespace within container repository
        
#### repository ####
 * Name of the target container repository

#### path_to_signing_key ###
 * Path of the public key that was used to sign the image

Requirements
------------
  See `../Pipfile` and `../setup.py`