orc
===

OCR Qt C++ server using tesseract. It can process PDF and some kind of images that **ImageMagick** can read. Connect to the server using socket (Port 5321) and base 64 encoding/decoding. The list of commands are:

### Installation

Just install **Tesseract** and **ImageMagick (convert)** command line tools. Then compile the OCR using **Qt Creator**. To test the system, use the **OCR Client** (https://github.com/paulocesar/ocr-client).

### Commands

##### Add document

Upload and add one document to the stack:
	
    add|http://www.tcpdf.org/examples/example_001.pdf
 
it will return a code like `5ac8395a9bed59cba8b6abc9`. Use this code to access the document's information.

##### Remove document

Remove the document and clean the source downloaded execute:

	remove|5ac8395a9bed59cba8b6abc9

##### Get processed page

Get the text from a processed pages:

	page|5ac8395a9bed59cba8b6abc9|<num_page>

##### Get information about the document

Get information about the document like number of pages and already processed pages:

	info|5ac8395a9bed59cba8b6abc9

##### Close connection

	quit
