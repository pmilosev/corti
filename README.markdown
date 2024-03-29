# CORTI

Utility for sound analysis and recognition using neural network.  
The name comes from the *organ of corti*: http://en.wikipedia.org/wiki/Organ_of_Corti

## NOTES

The project was mainly developed as part of my bachelor's degree final exam.  
As it usually gets with this stuff, there are much more options defined than implemented :)

I have open-sourced the code in a hope that someone would get interested and continue my work.  
There are no copyright headers in the code, and you are free to use it without any restrictions.  

## SYNOPSIS

<pre>
corti { &lt;COMMAND> &lt;OPTIONS> [&lt;KNOWLEDGE FILE> [&lt;SOUND FILE>]] | [-h | --help] [--version] }
</pre>

## DESCRIPTION

Corti is a command line utility that provides simple interface for sound analysis and sound recognition.
The interface consists of several commands and command dependant options.

The utility operates on so-called KNOWLEDGE files.
All parameters (like number of layers in the neural network) that are nesesary or can at any way change the behavior of the tool are stored here. Sound file can be any of the supported sound file types.

For complete list of commands and options see the COMMAND and OPTIONS sections below.
For complete list of supported sound file formats see the FORMATS section below.

## COMMAND

All commands listed below are mutual exclusive.

**NEW**   Creates new knowledge file.

	Valid options:  
		-v, --verbose
		-q, --quiet
		-t=&lt;T>, --window-type=&lt;T>
		-w=&lt;N>, --num-windows=&lt;N>
		-o=&lt;N>[%], --window-overlap=&lt;N>[%]
		-c=&lt;N>, --cepstral-coefitients=&lt;N>
		-e=&lt;D>, --max-error=&lt;D>
		-p=&lt;N>, --max-epoch=&lt;N>
		-r=&lt;D>, --learning-rate=&lt;D>
		-k=&lt;D>, --error-coefitient=&lt;D>
		-l=&lt;N>, --num-layers=&lt;N>
		-n=&lt;X>:&lt;N>, --num-neurons=&lt;X>:&lt;N>
		-m=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --weight=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>
		-b=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --bias=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>
		-s=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --sigmoid-coefitient=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>

	Example:
		Create new knowledge file with path "/home/corti/new_knowledge_file".
		Neural network has 3 x 13 = 39 input neurons,
		one hidden layer with 50 neurons,
		and 3 output neurons.
		Set all other values to their deffault values

		corti NEW -l=2 -n:1:50 -n:2:3 -w=3 -c=13 /home/corti/new_knowledge_file

**LIST**  List the content of the knowledge file.

	Valid options:
		-v, --verbose

	Example:
		List the entire knowledge file including network weights and saved lesons

		corti LIST -vv

**UPDATE**  Update knowledge parameters.

	Valid options:  
		-v, --verbose
		-q, --quiet
		--retrain
		-t=&lt;T>, --window-type=&lt;T>
		-w=&lt;N>, --num-windows=&lt;N>
		-o=&lt;N>[%], --window-overlap=&lt;N>[%]
		-c=&lt;N>, --cepstral-coefitients=&lt;N>
		-e=&lt;D>, --max-error=&lt;D>
		-p=&lt;N>, --max-epoch=&lt;N>
		-r=&lt;D>, --learning-rate=&lt;D>
		-k=&lt;D>, --error-coefitient=&lt;D>
		-l=&lt;N>, --num-layers=&lt;N>
		-n=&lt;X>:&lt;N>, --num-neurons=&lt;X>:&lt;N>
		-m=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --weight=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>
		-b=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --bias=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>
		-s=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --sigmoid-coefitient=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>

	Example:
		Modify knowledge file "/home/corti/new_knowledge_file".
		Set the learning rate to 0.7.
		Set the number of neurons into the second layer to 50.
		Retrain the network.

		corti UPDATE -r 0.7 -n 2:50 --retrain
		
**TRAIN**  Train neural network. Training is done on one sound file with one anwer. The answer is of format [x,y,z...] or [a=x,b=y,...]

	Valid options:
		--dry-run
		-v, --verbose
		-g, --graphicaly-verbose
		-q, --quiet
		--analyze
		--save
		-t=&lt;T>, --window-type=&lt;T>
		-w=&lt;N>, --num-windows=&lt;N>
		-o=&lt;N>[%], --window-overlap=&lt;N>[%]
		-c=&lt;N>, --cepstral-coefitients=&lt;N>
		-e=&lt;D>, --max-error=&lt;D>
		-p=&lt;N>, --max-epoch=&lt;N>
		-r=&lt;D>, --learning-rate=&lt;D>
		-k=&lt;D>, --error-coefitient=&lt;D>

	Example:
		Analyse the reconstruction of the frequency domain of the
		sound file "/home/corti/sound.aiff" using knowledge file
		"/home/corti/knowledge"

		corti TRAIN --analyze -gg -vv /home/corti/knowledge /home/corti/sound.aiff [0:0]

**DETECT**  Detect sound.

	Valid options:
		-v, --verbose
		-g, --graphicaly-verbose
		-q, --quiet
		--analyze
		-t=&lt;T>, --window-type=&lt;T>
		-w=&lt;N>, --num-windows=&lt;N>
		-o=&lt;N>[%], --window-overlap=&lt;N>[%]
		-c=&lt;N>, --cepstral-coefitients=&lt;N>
		-e=&lt;D>, --max-error=&lt;D>
		-p=&lt;N>, --max-epoch=&lt;N>
		-r=&lt;D>, --learning-rate=&lt;D>
		-k=&lt;D>, --error-coefitient=&lt;D>

	Example:
		Detect sound file "/home/corti/sound.aiff" 
		using knowledge file "/home/corti/knowledge"

		corti DETECT /home/corti/knowledge /home/corti/sound.aiff

## OPTIONS

**--dry-run**  Execute the command but make no changes.
Very usefull in combination with --verbose. It is a good practise to use this option before you do some critical changes into the knowledge file.

**-v, --verbose**  Explain what is been done.
You can specify this parameter twice if you like to see even more information.

**-g, --graphicaly-verbose**  Show graphics that can be constructed during the process (e.g. sound in time and spectral domain).
You can specify this parameter twice if you like to produce more graphics (e.g. reconstruction of the frequency and time domain from the cepstral coefitients)

**-q, --quiet**  Supress ALL messages.
Be carefull using this option because even if the process fails no errors will be printed (except for the system messages).
If at least one of the options --verbose or --graphicaly-verbose is provided --quiet have no effect.

**--analyze**  Same as --dry-run except only the steps for preparing data are made (i.e. data is not fed into the neural network)
This option together with --verbose and --graphicaly-verbose can be used to analyze the sound and parameters before any real training or detection is done.

**--save**  Save the computed neural network input into the knowledge file.
Later if the structure of the network is changed, saved values will be used to re-train it.

**--retrain**  Re-train the neural network using saved values.

**-t=&lt;T>, --window-type=&lt;T>**  Window type to be used when windowing the input sound.

	&lt;T> is one of:
		rect	 - rectangular window
		gaus	 - gauss window
		hamming	 - hamming window
		hann	 - hann window
		bartlett - bartlett window (zero valued end-pionts)
		triang	 - triangular window (non-zero end-points)
		bar_hann - bartlett-hann window
		blackman - blackman window

**-w=&lt;N>, --num-windows=&lt;N>**  Number of windows to be aplied on the input sound.
This number multipled with the number of cepstral coefitients taken from each window provide the number of inputs into the neural network.

**-o=&lt;N>[%], --window-overlap=&lt;N>[%]**  Number of samples (or percents) shared between two consequentive windows.

**-c=&lt;N>, --cepstral-coefitients=&lt;N>**  Number of cepstral coefitients taken from each window.

**-e=&lt;D>, --max-error=&lt;D>**  Maximum error alowed. &lt;D> should be floating point number 0 &lt; D &lt; 1
Training of the network stops if the output of the network produces less or equal error then the maximum error specified.

**-p=&lt;N>, --max-epoch=&lt;N>**  Mazimum number of epoch the network is alowed to spend on one lesson when training.
If the lesson is not learned within this period it is assumed that the lesson can not be learned and next lesson is started.
One epoch actualy is one pass of the input data trought the network.

**-r=&lt;D>, --learning-rate=&lt;D>**  Learning rate parameter for the neural network. &lt;D> could be any floating point number but usualy values around 0.5 are used. If learning rate is to small network could be stuck in a local minimum. By contrast if the learning rate is to high then the network could jump over the global minimum.

**-k=&lt;D>, --error-coefitient=&lt;D>**  Error coefitient. &lt;D> could be any floating point number but usualy values around 0.5 are used. If error coefitient is to small the network will most probably need more epochs to learn the lesson because the wight corections will be minimal. By contrast if the error coefitient is to high the weight corections will just jump over the correct values.

**-l=&lt;N>, --num-layers=&lt;N>**  Number of layers the network consists of. &lt;N> could be any positive integer but values above 4 are not common and will lead to slow network performance.

**-n=&lt;X>:&lt;N>, --num-neurons=&lt;X>:&lt;N>**  Number of neurons the X-th layer consists of. &lt;N> could be any positive integer. The number of neurons in the last layer depends on the type of the answer, most comonly it is same with the number of classes in wich the data is divided to. The number of neurons in each hidden layer is one of the most important parameters that could lead from great to catastrofic results. Setting this number to small will most probably lead to false classification, however setting it to high will increase the computation time and also some times lead to false classification. Most commonly you would like to start from smaller number and watch how the increasing of this number reflect the performance of the network. It is most common that the inner layers consists of more neurons then the outer ones. 

**-m=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --weight=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>**  Input weight value. The &lt;X>,&lt;Y> and &lt;Z> numbers represent the weight's position i.e. the laer, neuron and chanal respectively. If they are not provided then all weights are set to value &lt;D>. If some of them equals 0 then all weights covered by the rest of the coordinates are set to &lt;D> (e.g. -m=5:0:0:0.5 will set all weights in the fifth layer to 0.5).
&lt;D> could be any floating point number but usualy values 0 &lt; D &lt; 1 are used. By deffault all weights are set to random numbers in the interva [0,1].

**-b=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --bias=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>**  Neuron's bias value. For explanation of the &lt;X>,&lt;Y> and &lt;Z> numbers see the -m option above. &lt;D> could be any floating point number but usualy values around 0 are used. If one neuron have greater bias then some other neuron from the same layer then it has greater influence on the resulting value. By deffault all neurons are assumed equal with their biasis set to 0.

**-s=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>, --sigmoid-coefitient=[&lt;X>:&lt;Y>:&lt;Z>:]&lt;D>**  Neuron's sigmoid coefitient. For explanation of the &lt;X>,&lt;Y> and &lt;Z> numbers see the -m option above. 
&lt;D> could be any floating point number but usualy values around 1 are used. This parameter have the folowing meanign into the sigmoid function:

		sig = 1 / (1 + exp(-&lt;D> * H))

	Changing this values affects the activation function of the neuron. By deffault it is set to 1.

**-h, --help**  Displays short help and exit.

**--version**  Displays version and exit.

## AUTHOR

Pece Milosev
