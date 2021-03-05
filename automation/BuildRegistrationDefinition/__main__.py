# -*- coding: utf-8 -*-

#
# MIT License
#
# Copyright (c) 2021 International Business Machines
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

"""
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
"""

import getpass
import glob
import json
import os
import subprocess
import sys
from pathlib import Path
from subprocess import CalledProcessError
from typing import Dict, List

import click

from BuildRegistrationDefinition import __title__, __version__

CONTEXT_SETTINGS: dict = dict(help_option_names=["-h", "--help"],)

TEMPLATE_FILENAME: str = "registration_definition.template"

SCRIPT_PATH: str = os.path.dirname(os.path.abspath(__file__))


@click.command(context_settings=CONTEXT_SETTINGS)
@click.version_option(
    version=__version__,
    prog_name=__title__,
    message="%(prog)s version %(version)s",
    help="Show version information and exit",
)
@click.argument("registry", type=click.STRING)
@click.argument("namespace", type=click.STRING)
@click.argument("repository", type=click.STRING)
@click.argument("path_to_signing_key", type=click.Path(exists=True, dir_okay=False))
@click.option(
    "--dockerid",
    type=click.STRING,
    required=False,
    help="Userid to access container repository; if omitted, will assume the same value as NAMESPACE ",
)
@click.option(
    "--dockerpass",
    type=click.STRING,
    required=False,
    help="Password to access container repository; will prompt if omitted",
)
@click.option(
    "--outputfile",
    type=click.Path(),
    required=False,
    help="Path to write output JSON file; will be written to $PWD/hpvs-fhe-registration.txt if omitted",
)
def main(**kwargs) -> None:
    """
    Build and sign a Hyper Protect Virtual Server registration definition file
    
    \f
    
    Parameters
    ----------
    registry : str
        Name of target container registry.  Acceptable values are `hub`,
        `docker.io`, or the registry's URL
    namespace : str
        Target namespace within container repository
    repository : str
        Name of the target container repository
    dockerid : str
        Userid to access container repository.  **Optional** - if omitted, will
        assume the same value as `namespace`
    dockerpass : str
        Password to access container repository.  **Optional** - will prompt if
        omitted
    path_to_signing_key : str
        Path of public key file that was used to sign the image.
    outputfile : str
        Path to write output JSON file.  **Optional** - If omitted, JSON output
        will not be written to a file.
    
    Returns
    -------
    None
        JSON output will be displayed on STDOUT.  If `outputfile` was specified,
        JSON output will additionally be written to a file at the path
        indicated.
    """

    #
    # Local data members
    #
    public_key: str = None
    """Public key that was used to sign the container image"""

    registration_definition: Dict = None
    """Dictionary containing the registration definition"""

    repo_inspect: Dict = None
    """`docker inspect` output for the target repository"""

    repo_name: str = None
    """Full repository name, including namespace"""

    repo_uri: str = None
    """URL to the target container repository"""

    root_key_id: str = None
    """Root key ID that was used to sign the container image"""

    vendor_key: str = None
    """Vendor public key used to verify the registration definition file"""

    # Load and validate the registration definition template file, located in
    # the same directory as this source file
    registration_definition = _load_template_file()
    if not registration_definition:
        sys.exit(1)

    # Build the full repository name
    repo_name = f"{kwargs['namespace']}/{kwargs['repository']}"
    if kwargs["registry"] == "hub":
        repo_uri = repo_name  # "hub.docker.com/repository/docker/{repo_name}"
    elif kwargs["registry"] == "docker.io":
        repo_uri = f"docker.io/{repo_name}"
    else:
        repo_uri = f"{kwargs['registry']}/{repo_name}"
    registration_definition["repository_name"] = repo_uri

    # If the repository userid wasn't specified, assume its the same as the namespace
    if "dockerid" in kwargs and kwargs["dockerid"]:
        registration_definition["docker_username"] = kwargs["dockerid"]
    else:
        registration_definition["docker_username"] = kwargs["namespace"]

    # If the repository password  was not specified, prompt the user for it
    if "dockerpass" not in kwargs or kwargs["dockerpass"]:
        registration_definition["docker_password"] = kwargs["dockerpass"]
    else:
        prompt: str = f"Password for user {kwargs['dockerid']} on container registry '{kwargs['registry']}': "
        registration_definition["docker_password"] = getpass.getpass(prompt)

    # Run `docker trust inspect` and copy the Root key ID from the
    # `AdministrativeKeys` that were used to sign the container image
    # on your build system
    repo_inspect = _inspect_docker_repo(repo_uri)
    if not repo_inspect:
        sys.exit(1)
    root_key_id = _extract_root_key_id(repo_inspect)
    if not root_key_id:
        sys.exit(1)
    registration_definition["public_key_id"] = root_key_id

    # Get the public key from the container metadata file and
    # copy it into the public_key field
    public_key = _extract_public_key(repo_name, root_key_id)
    if not public_key:
        sys.exit(1)
    registration_definition["public_key"] = public_key

    # Get the vendor public key
    vendor_key = _load_vendor_key(kwargs["path_to_signing_key"])
    if not vendor_key:
        sys.exit(1)
    registration_definition["vendor_key"] = vendor_key

    # Convert the registration_definition into JSON format
    data = json.dumps(registration_definition, sort_keys=True, indent=3)

    outputfile: str = None
    if kwargs["outputfile"]:
        outputfile = kwargs["outputfile"]
    else:
        outputfile = f"{os.cwd()}/hpvs-fhe-registration.txt"
    with open(outputfile, "w") as f:
        f.write(data)
    sys.exit(0)


def _load_template_file() -> Dict:
    """
    Read and validate the registration definition template file, located in the
    same directory as this source file
    
    Returns
    -------
    Dict
        Contents of the registration definition template file JSON, converted to
        a Python dictionary
    """
    src_path: str = f"{SCRIPT_PATH}/{TEMPLATE_FILENAME}"

    if not os.path.exists(src_path):
        print("Error: Cannot find registration definition template file")

    with open(src_path) as registration_definition_template_file:
        try:
            data = registration_definition_template_file.read()
        except:
            print("Error: Cannot read registration definition template file")
            return None

    # Reconstruct the dict object
    readin = json.loads(data)
    if type(readin) != dict:
        print("Error: Corrupted registration definition template file")
        return None

    return readin


def _load_vendor_key(keyfile_path: str) -> str:
    """
    Read and validate a vendor key file
    
    Parameters
    ----------
    keyfile_path : str
        Path to the vendor key file
    
    Returns
    ------
    str
        Contents of the vendor key file
    """
    data: str = None

    if not os.path.exists(keyfile_path):
        print(f"Error: Cannot find vendor key file '{keyfile_path}'")

    with open(keyfile_path) as vendor_key_file:
        try:
            data = vendor_key_file.read()
        except:
            print(f"Error: Cannot read vendor key file '{keyfile_path}'")

    return data


def _inspect_docker_repo(repo_name: str) -> Dict:
    """
    Run `docker trust inspect` on the named container repository

    Parameters
    ----------
    repo_name : str
        Full name of target contaioner repository name, of the form:
            `repository_name/image_name`
    
    Returns
    -------
    Dict
        Low-level information about keys and signatures associated with the
        named container repository
    """
    result: Dict = None

    try:
        completed_process: CompletedProcess = subprocess.run(
            ["docker", "trust", "inspect", repo_name],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
        )
        di_output = json.loads(completed_process.stdout)[0]
        if type(di_output) != dict:
            print(f"Error: Problem inspecting the container image '{repo_name}'")
        else:
            result = di_output
    except CalledProcessError as e:
        print(str(e))
        print(f"Error: Unable to inspect the container image '{repo_name}'")
    finally:
        return result


def _extract_root_key_id(repo_inspect: Dict) -> str:
    """
    Extract the root signing key from the trust inspection data
    
    Parameters
    ----------
    repo_inspect : Dict
        JSON-formatted trust inspection data
    
    Returns
    -------
    str
        The ID of the root signing key contained within `repo_inspect`
    """

    if "AdministrativeKeys" in repo_inspect:
        for key_item in repo_inspect["AdministrativeKeys"]:
            if key_item["Name"] == "Root":
                for key in key_item["Keys"]:
                    if "ID" in key:
                        return key["ID"]

    print(f"Error: Unable to find ID of Root signing key for targeted repository")
    return None


def _extract_public_key(repo_name: str, root_key_id: str) -> str:
    """
    Open the metadata file `~/.docker/trust/tuf/metadata/root.json` and
    locate public key for the container image by searching for its' Root key ID.
    
    Parameters
    ----------
    repo_name : str
        Full repository name, including namespace
    root_key_id : str
        Root key ID that was used to sign the container image
    
    Returns
    -------
    str
        Public key for the container
    """

    # Find all files named root.json in the file tree anchored at ~/.docker
    for path in glob.glob(f"{Path.home()}/.docker/**/root.json", recursive=True):
        if repo_name in path:
            if path.endswith(f"{repo_name}/metadata/root.json"):
                with open(path) as root_json_file:
                    try:
                        # Reconstruct the dict object
                        readin = json.loads(root_json_file.read())
                        if type(readin) != dict:
                            print(
                                "Error: Corrupted registration definition template file"
                            )
                        else:

                            # Locate the entry containing the image's Root key ID
                            docker_metadata: Dict = readin
                            if (
                                "signed" in docker_metadata
                                and docker_metadata["signed"]
                            ):
                                signed_keys: Dict = docker_metadata["signed"]
                                if "keys" in signed_keys and signed_keys["keys"]:
                                    if root_key_id in signed_keys["keys"]:
                                        return signed_keys["keys"][root_key_id][
                                            "keyval"
                                        ]["public"]
                    except:
                        print("Error: Cannot read container metadata file")

    return None


if __name__ == "__main__":
    main()
