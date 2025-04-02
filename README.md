# Projekt: Szeregowanie Zadań na Pojedynczym Procesorze

## Opis problemu
Problem polega na optymalnym szeregowaniu zadań na pojedynczym procesorze, gdzie każde zadanie ma trzy kluczowe parametry:
- **rj** - czas dostępności zadania
- **pj** - czas wykonania zadania
- **qj** - czas stygnięcia po zakończeniu

Celem jest minimalizacja maksymalnego czasu zakończenia zadania (makespan, **Cmax**).

---

## Implementowane Algorytmy

### 1. Przegląd Zupełny 
Algorytm generuje wszystkie permutacje kolejności wykonywania zadań i wybiera tą, która daje najkrótszy czas zakończenia wszystkich zadań (**Cmax**).

### 2. Algorytm Heurystyczny - Sortowanie po rj
Zadania są sortowane rosnąco według czasu dostępności (**rj**), co oznacza, że priorytetowo obsługiwane są zadania, które można wykonać najwcześniej.

### 3. Algorytm Heurystyczny - Sortowanie po qj
Zadania są sortowane malejąco według czasu stygnięcia (**qj**), co powoduje priorytetowe wykonywanie zadań z największym czasem stygnięcia.

### 4. Algorytm "Kolejność hybrydowa" (rj + qj)

## Opis algorytmu

Algorytm hybrydowy ma na celu minimalizację całkowitego czasu zakończenia wszystkich zadań (Cmax), łącząc dwa podejścia:
1. Sortowanie według czasu dostępności `rj` (rosnąco) – aby przetwarzać zadania w kolejności ich dostępności.
2. Wybieranie spośród dostępnych zadań tego, które ma największą wartość `qj` – aby minimalizować wpływ stygnięcia na końcowy wynik.

## Kroki algorytmu

1. **Inicjalizacja**:
   - Pobieramy liczbę zadań `n`.
   - Tworzymy wektor `order`, w którym umieszczamy indeksy zadań.
   
2. **Sortowanie początkowe**:
   - Sortujemy `order` według `rj` w kolejności rosnącej.

3. **Główna pętla przetwarzania**:
   - Tworzymy pusty wektor `finalOrder`, który przechowuje kolejność wykonywania zadań.
   - Ustawiamy `currentTime = 0`, czyli początkowy czas systemu.
   - Tworzymy pusty wektor `available`, który przechowuje indeksy dostępnych zadań.
   - Inicjalizujemy zmienną `index = 0`, aby śledzić, które zadania zostały już dodane do `available`.

4. **Przetwarzanie zadań**:
   - Dopóki nie przetworzymy wszystkich `n` zadań:
     - Dodajemy do `available` wszystkie zadania z `order`, które mają `rj <= currentTime`.
     - Jeśli `available` nie jest puste:
       - Znajdujemy w `available` zadanie o największym `qj`.
       - Dodajemy je do `finalOrder`.
       - Usuwamy je z `available`.
       - Aktualizujemy `currentTime` o jego czas przetwarzania `pj`.
     - Jeśli `available` jest puste:
       - Ustawiamy `currentTime` na `rj` pierwszego niewykonanego zadania.

5. **Obliczenie `Cmax`**:
   - Wyznaczamy `Cmax` dla powstałego harmonogramu za pomocą funkcji `getCmax()`.

6. **Zwrócenie rozwiązania**:
   - Zwracamy najlepszy harmonogram oraz jego `Cmax`.

## Złożoność obliczeniowa

1. **Sortowanie według `rj`** – `O(n log n)`.
2. **Wybór maksymalnego `qj`** w każdej iteracji – `O(n^2)`, ponieważ w najgorszym przypadku musimy za każdym razem przeszukać całą listę dostępnych zadań.
3. **Całkowita złożoność** – `O(n^2)`, co sprawia, że algorytm działa efektywnie dla umiarkowanej liczby zadań.



## Wnioski
Algorytm "Kolejność hybrydowa" jest dobrym kompromisem pomiędzy prostotą a skutecznością. Dzięki kombinacji `rj` i `qj` pozwala znaleźć lepsze harmonogramy niż klasyczne heurystyki, ale nadal nie gwarantuje optymalnego rozwiązania jak przegląd zupełny.


---

## Struktura Kodu

Plik główny zawiera klasy i funkcje implementujące powyższe algorytmy:

### Klasy
#### `Task`
Reprezentuje pojedyncze zadanie.
```cpp
class Task{
public:
    int rj, pj, qj;
    Task(int r, int p, int q) : rj(r), pj(p), qj(q) {}
};
```

#### `Problem`
Przechowuje listę zadań i umożliwia ich dodawanie.
```cpp
class Problem {
public:
    std::vector<Task> tasks;
    void addTask(int r, int p, int q) {
        tasks.emplace_back(r, p, q);
    }
};
```

#### `Solution`
Przechowuje kolejność wykonywania zadań oraz wartość **Cmax**.
```cpp
class Solution{
public:
    std::vector<int> order;
    int cmax;
    Solution() : cmax(INT_MAX) {}
};
```

### Funkcje

#### `getCmax`
Oblicza maksymalny czas zakończenia zadań dla danej kolejności.
```cpp
int getCmax(const std::vector<Task> &tasks, const std::vector<int> &order);
```

#### `getBestSolution`
Zastosowanie przeglądu zupełnego do znalezienia optymalnej kolejności.
```cpp
Solution getBestSolution(Problem &problem);
```

#### `heuristicSort`
Sortowanie heurystyczne według **rj** lub **qj**.
```cpp
Solution heuristicSort(Problem &problem, const std::string& sortBy);
```

#### `hybridSort`
Wykorzystuje sortowanie według **rj**, a następnie selekcjonuje zadania o najwyższym **qj**.
```cpp
Solution hybridSort(Problem &problem);
```

#### `testInstance`
Funkcja testująca działanie algorytmów na przykładowych danych.
```cpp
void testInstance();
void testInstance(const std::string& filename);
```

---

## Testowanie

Aby przetestować algorytmy, można uruchomić funkcję `testInstance()` w `main()`:
```cpp
int main() {
    testInstance();
    return 0;
}
```
Można też wczytać dane z pliku:
```cpp
// testInstance("data.txt");
```
Plik wejściowy powinien mieć format:
```
6
1 5 9
4 5 4
1 4 6
7 3 3
3 6 8
4 7 1
```
Gdzie pierwsza liczba to liczba zadań, a każda kolejna linia to `rj pj qj`.

---

## Wyniki i Analiza
Każdy algorytm zwraca kolejność zadań i wartość **Cmax**:
```
Best cmax with permutation: 27
Order: 2 0 3 1 5 4
Best cmax with sort by rj: 30
Order: 0 2 4 1 5 3
Best cmax with sort by qj: 32
Order: 5 4 2 3 1 0
Best cmax with own algorithm: 29
Order: 2 4 0 5 3 1
```

---

## Podsumowanie
Zaimplementowane algorytmy umożliwiają znalezienie optymalnego **Cmax** przy użyciu przeglądu zupełnego oraz heurystyk. Najlepsze wyniki osiąga **Kolejność Hybrydowa**, która łączy sortowanie po **rj** i wybór według **qj**.




