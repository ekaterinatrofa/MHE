# Problem sumy podzbioru

Problem sum podzbiorów jest jednym z dobrze znanych problemów w teorii algorytmów i dobrym przykładem problemów NP-zupełnych.

# Treść problemu

Definicja problemu polega na stwierdzeniu, że suma dowolnego podzbioru danego zestawu liczb dodatnich jest sumą docelową.

# Przykład

```sh
Dla zbioru [1,2,3,4,5] możemy stworzyć podzbiory sumujące się do 5 (np. [5], [2,3], [1,4])
```

# Struktury danych

Zbiory liczb są vectorami elementów typu `double`.

Na wejściu dostajemy zbiór liczb wczytany z pliku tekstowego oraz poszukiwaną sumę.

Na wyjściu dostajemy dowolny podzbiór sumujący się do poszukiwanej liczby lub pusty zbiór.

