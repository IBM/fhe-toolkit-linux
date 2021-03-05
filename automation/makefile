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

pypi_url=https://pypi.org/simple
repo=`grep __title__ ./BuildRegistrationDefinition/__version__.py | grep -o '"[^"]\+"' | cut -d '"' -f2`
version=`grep __version__ ./BuildRegistrationDefinition/__version__.py | grep -o '"[^"]\+"' | cut -d '"' -f2`

help: intro
	@echo ""glad
	@echo "Available makefile targets:"
	@echo ""
	@echo "    build"
	@echo "    clean"
	@echo "    clean-doc"
	@echo "    dev"
	@echo "    doc"
	@echo "    init"
	@echo "    install"
	@echo "    intro"
	@echo "    sort"
	@echo "    test"
	@echo "    tidy"
	@echo "    uninstall"
	@echo ""

intro:
	@echo "$(repo) v$(version)"

init:
	pip3 install pipenv 
	pipenv install --dev
	@echo "Now run: pipenv shell"

clean:
	@rm -rf ./build ./dist ./__pycache__/
	@rm -rf ./$(repo)/$(repo).egg-info ./.eggs ./$(repo).egg-info
	@rm -f *.pyc *.pyo *.pyd *\$$py.class
	@find . -name "*.pyc" -exec rm -f {} \;
	@rm -rf ./BuildRegistrationDefinition-* *.log *.txt

clean-doc:
	@rm -rf ./doc
	
doc:
	@pipenv run pdoc --force -o ./doc $(repo)
	mv ./doc/BuildRegistrationDefinition/__main__.md ./BuildRegistrationDefinition/README.md
	rm -rf ./doc

test:
	@python3 -m pytest

dev: intro
	@echo ""
	@pipenv shell

build: init doc
	@pipenv run pipenv-setup sync
	@pipenv run python3 setup.py sdist bdist_xar

install: init
	@pipenv run pipenv-setup sync
	@pip3 install .

uninstall:
	echo "pip3 uninstall $(repo)==$(version) -y"
	@pip3 uninstall $(repo)==$(version) -y

sort:
	isort -rc --atomic .

tidy: sort
	black .
	
MAKE:
	help

.PHONY: build clean clean-doc makdev doc help init install intro sort test tidy uninstall
