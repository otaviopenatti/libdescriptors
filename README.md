# Lib Descriptors

This repository has the source codes and scientific papers of a set of global image descriptors (color and texture).
These descriptors were analyzed and compared experimentally during my masters degree at Institute of Computing (IC) from University of Campinas (Unicamp), in Brazil.
My masters studies were supervised by professor Ricardo da Silva Torres from 2007 to 2009.

The paper below has one of the main results of my masters dissertation which is a theoretical and experimental evaluation of these descriptors.
If you use any code from this repository, please cite the following paper:

"Comparative study of global color and texture descriptors for web image retrieval." 
Penatti, Otávio A. B., Eduardo Valle, and Ricardo da S. Torres. 
Journal of visual communication and image representation 23, no. 2 (2012): 359-380.

```latex
Bibtex:
@article{penatti2012comparative,
  title={Comparative study of global color and texture descriptors for web image retrieval},
  author={Penatti, Ot{\'a}vio AB and Valle, Eduardo and Torres, Ricardo da S},
  journal={Journal of visual communication and image representation},
  volume={23},
  number={2},
  pages={359--380},
  year={2012},
  publisher={Elsevier}
}
```

### Descriptors included:
**Color:**
- Color autocorrelogram (ACC)
- Border-interior Pixel Classification (BIC)
- Color-based Clustering (CBC)
- Color Coherence Vectors (CCV)
- Cumulative Global Color Histogram (CGCH)
- Color Bitmap (colorbitmap)
- Color Structure Descriptor (CSD)
- Color Wavelet HSV (CW-HSV)
- Color Wavelet LUV (CW-LUV)
- Global Color Histogram (GCH)
- Joint Autocorrelogram (JAC)
- Local Color Histogram (LCH)

**Texture:**
- Color Co-Occurrence Matrix (CCOM)
- Homogeneous Texture Descriptor (HTD)
- Local Activity Spectrum (LAS)
- Local Binary Patterns (LBP)
- Multi-scale Steerable Pyramid Decomposition (M-Spyd)
- Quantized Compound Change Histogram (QCCH)
- Statistical Analysis of Structural Information (SASI)
- Sum and Difference Histogram (Unser)


**Each descriptor has:**
- Source code (C language)
- Paper (PDF)

**How descriptors work (very high-level view):**
1. Extraction: the extraction program extracts a feature vector from a single image (output is a file containing the feature vector)
2. Distance: the distance program computes the distance between two feature vectors (output is printed on standard output)

**Remarks:**
- Descriptors were compiled and tested on GNU/Linux environment, specifically on Ubuntu 18.04.3 LTS.
- Each descriptor's source-code package works independently of the other descriptors. So you can download and compile only the descriptors of your interest (all the code necessary to use it is already in the "source" folder). Consequently, however, code (basic functions) is replicated across descriptors.

**How to compile and use a descriptor:**
1. To compile a descriptor, enter the "source/app" directory and run "make"
2. The main executables will be generated in the "source/bin" directory
3. Three programs will be generated after compilation
	1. Feature vector extractor
	2. Distance calculator
	3. Descriptor plugin in case the user wants to use the descriptor in the Eva tool (or to embed as library in any other code)
4. Run each executable with no arguments for instructions
	Ps.: to install a plugin on Eva tool, consider the instuctions on the tool itself

### License:
All the code here is under GPL v3.

### Acknowledgements:
I would like to thank all the people who contributed to the implementation of the image descriptors.

The source code was based on the image processing library from professor Alexandre Xavier Falcão, from Institute of Computing (IC) at University of Campinas (Unicamp).

Please, find below the names of the people who contributed to the descriptors' implementations:
- Fabio Augusto Faria - colorbitmap, QCCH
- Felipe B. Valio - LAS
- Felipe S. P. Andrade - CSD, CCOM
- Fernando Cesar Akune - CSD, CCOM
- Jurandy Almeida - BIC, ACC, GCH, LCH
- Lucas Bueno - SASI, LBP
- Nadia Kozievitch - JAC
- Ricardo Panaggio - CW-LUV, CCOM
- Rodrigo Rezende - JAC, HTD
- Rodrigo Tripodi
- Thiago Spina - M-Spyd

I also thank the RECOD lab from IC-Unicamp for the computational infrastructure for organizing, compiling and testing all the descriptors.

**Known bugs:**
- distance functions cannot handle files that were not extracted by their own; some descriptors crash, some compute invalid numbers


