# OS_project3_Thread-Synchronization
3 threads synchronization &amp; communication by named pipe (C lang, linux platform)

<h2>2 Programs :</h2>
1) numberreader.c : </br>
- works as service,reads string sent by 'xyztotlizer' from named pipe, splits character and number.</br>
- the number is line number, reads that line then writes it to the named pipe.</br>
- when End Of File :writes 'END' to the pipe so xyztotlizer exites.</br>
2) xyztotalizer.c :</br>
- lets user enter Read Count number and reads from number.txt as Read Count.</br>
- Sends x/y/z in order and line to be read as one string (ex:y2) to the pipe.</br>
- Reads value written back by Numberreader from the pipe and assigns it to current character(x/y/z)</br>
- when Z : print the sum x+y+z and set current char x.</br>

* mutex lock mechanism is used for thread synchronization. (xthread,ythread,zthread)</br>

<h2>Tips:</h2>
<i>Download files</i>
<i>In Terminal : </br>
  >>makeFile (to compile c files) </br>
  >> ./numberreader (ready to recieve values from pipe) </br>
  in another terminal :</br>
  >> ./xyztotlizer </br>
  >>(enter read count)(3/6/9..)</br>
  +screenshots in the<a href= "https://github.com/AlaaMarawi/OS_project3_Thread-Synchronization/blob/master/Rapor.pdf"> report</a>.
</i>
</br>
