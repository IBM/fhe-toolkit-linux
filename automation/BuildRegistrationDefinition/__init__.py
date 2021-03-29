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
.. include:: ../README.md
"""

from . import __version__ as version_info

__version__ = version_info.__version__
__author__ = version_info.__author__
__author_email__ = version_info.__author_email__
__title__ = version_info.__title__
__description__ = version_info.__description__
__url__ = version_info.__url__
__build__ = version_info.__build__
__license__ = version_info.__license__
__copyright__ = version_info.__copyright__
__keywords__ = version_info.__keywords__
__project_urls__ = version_info.__project_urls__

# Tell pdoc to explicitly include __main__.py
__pdoc__ = {
    "BuildRegistrationDefinition.__main__": True,
}
