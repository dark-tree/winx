
# MIT License
#
# Copyright (c) 2022 magistermaks
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

import argparse
import zipfile

parser = argparse.ArgumentParser( description="WXGL Generator" )
parser.add_argument("--glad", help="Specify path to GLAD zip file", type=str, required=True)
glad_zip_path = parser.parse_args().glad

with zipfile.ZipFile(glad_zip_path, "r") as zip:

	# glad sources
	glad_source = zip.read("glad.c").decode("utf-8")
	glad_header = zip.read("glad.h").decode("utf-8")
	khrplatform_header = zip.read("khrplatform.h").decode("utf-8")

# winx files
winx_source = open("../winx.c", "r").read()
winx_header = open("../winx.h", "r").read()
wxgl_source = open("template/wxgl.c", "r").read()
wxgl_header = open("template/wxgl.h", "r").read()

# those glad includes are used by the "local files" version only
glad_header = glad_header.replace("#include \"khrplatform.h\"", khrplatform_header)
glad_source = glad_source.replace("#include \"glad.h\"", "\n")
winx_source = winx_source.replace("#include \"winx.h\"", "\n")

wxgl_header = wxgl_header.replace("//REPLACE_GLAD_HEADER//", glad_header)
wxgl_header = wxgl_header.replace("//REPLACE_WINX_HEADER//", winx_header)

wxgl_source = wxgl_source.replace("//REPLACE_GLAD_SOURCE//", glad_source)
wxgl_source = wxgl_source.replace("//REPLACE_WINX_SOURCE//", winx_source)

open("wxgl.c", "w").write(wxgl_source)
open("wxgl.h", "w").write(wxgl_header)

