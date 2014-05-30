jftt2014-compiler
=================

realizacja projektu zaliczeniowego na kurs Języki formalne i techniki translacji - kompilator

### Budowanie

+ system budowania [CMake](http://www.cmake.org/).
+ prerekwizyty:
  + [CMake](http://www.cmake.org/) ``>= 2.6``
  + kompilator **C++** zgodny ze standardem **C++11**, (testowane na ``gcc == 4.9.0``)
  + [bison](http://www.gnu.org/software/bison/) (testowane na ``3.0.2``)
  + [flex](http://flex.sourceforge.net/) (testowane na wersji ``2.5.39``)
  + [gmp](https://gmplib.org/) ``>= 6.0.0`` (testowane na wersji ``6.0.0``)
  + ``gmpxx`` - C++ wrapper na ``gmp``, trzeba odpowiednio skompilować ``gmp``

#### Konfiguracja
Żeby wygenerować odpowiednio skonfigurowany plik *Makefile* należy wpisać komendę
```
cmake CMakeLists.txt
```

Domyślnie program zostanie skonfigurowany pod budowanie wersji produkcyjnej (*release*), w celu zbudowania wersji deweloperskiej należy dopisać ``-DCMAKE_BUILD_TYPE=debug``. Ponadto domyślnie maszyna wirtualna nie jest budowana. By ją zbudować dopisz ``-DBUILD_VM=on``.

#### Kompilacja
Gdy system budowania wygeneruje wszystkie pliki wystarczy wpisać
```
make
```

by rozpocząć proces kompilacji. Dodanie opcji ``-j N`` spowoduje uruchomienie kompilacji na *N* wątkach, a ``VERBOSE=1`` szczegółowego opisu wykonywanych czynności. 

Po kompilacji wszystko będzie znajdować się w podkatalogu ``build/``.

### Obsługa programu
Kompilator czyta plik wejściowy ze standardowego wejścia (*stdin*) i zwraca kod na maszynę wirtualną na standardowe wyjście (*stdout*). Wszystkie logi są wysyłane na *stderr*. Przykładowe wywołanie kompilatora dla jednego z plików z załączonych przykładów:
```
?> cat examples/moje/divide-0.imp | build/bin/kompilator > divide-0.mr
```

=================

### Uwagi

+ Kod udostępniany jest na licencji MIT, z wyjątkiem plików zawartych w podkatalogu `/vm/`, którego nie jestem autorem. Pliki umieszczone w tym katalogu zostały udostępnione przez prowadzącego do wykorzystania w celach naukowych. Wszelkie informacje na temat ich licencjonowania można uzyskać od ich autora.
