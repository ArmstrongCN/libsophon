This is the document repo of NNToolChain, which is used for deploying models on SOPHON deep-learning processors (https://sophon.ai).

* Frontend parsers for Caffe/TensorFlow/MXNet/PyTorch
* Graph optimization compiler and runtime
* Layer-based API: bmdnn (similar to cuDNN for NVIDIA GPU);
* Tensor/Operator-based API: bmlang (writing custom op);

## Prerequisites

install **sphinx** texlive-xetex texlive-latex-recommended

```shell
pip install sphinx sphinx-autobuild sphinx_rtd_theme rst2pdf
```

## Get Fandol font for linux system

* [Fandol](https://ctan.org/pkg/fandol) - Four basic fonts for Chinese typesetting

Download the fandol package from the aboved website(wget http://mirrors.ctan.org/fonts/fandol.zip) and unzip it.

Copy the fold named fandol to /usr/share/fonts or ${HOME}/.fonts.

If fandol exists in /usr/share/fonts, we can ignore this step.

## Compilation

```shell
## build html
make html # build document to static html files
python3 -m http.server --directory build/html
# open http://localhost:8000 in browser

## make  pdf

```

## Testing

TODO

## Deployment

TODO

## Built With

* [Sphinx](http://www.sphinx-doc.org) - Document auto generate tool
* [Latex](https://www.latex-project.org/) - High-quality typesetting system

## License

This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments
