#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <functional>
using namespace std;
using namespace std::chrono;

// Selection Sort 
// Time : O(n²) best/avg/worst
// Space: O(1) 
void selectionSort(vector<int>& a, long long& c, long long& s) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) {
            c++;
            if (a[j] < a[m]) m = j;
        }
        if (m != i) { swap(a[i], a[m]); s++; }
    }
}
//Bubble Sort 
// Time : O(n) best, O(n²) avg/worst
// Space: O(1) 
void bubbleSort(vector<int>& a, long long& c, long long& s) {
    int n = a.size();
    bool sw;
    for (int i = 0; i < n - 1; i++) {
        sw = false;
        for (int j = 0; j < n - i - 1; j++) {
            c++;
            if (a[j] > a[j + 1]) { swap(a[j], a[j + 1]); s++; sw = true; }
        }
        if (!sw) break;   // early exit: already sorted
    }
}
// Quick Sort 
// Time : O(n log n) avg, O(n²) worst (sorted input + last-element pivot)
// Space: O(log n) 
int partition(vector<int>& a, int lo, int hi, long long& c, long long& s) {
    int piv = a[hi], i = lo - 1;
    for (int j = lo; j < hi; j++) {
        c++;
        if (a[j] <= piv) { swap(a[++i], a[j]); s++; }
    }
    swap(a[i + 1], a[hi]); s++;
    return i + 1;
}

void quickSort(vector<int>& a, int lo, int hi, long long& c, long long& s) {
    if (lo < hi) {
        int p = partition(a, lo, hi, c, s);
        quickSort(a, lo, p - 1, c, s);
        quickSort(a, p + 1, hi, c, s);
    }
}
// Merge Sort
// Time : O(n log n) always
// Space: O(n)  
void merge(vector<int>& a, int l, int m, int r, long long& c, long long& s) {
    vector<int> L(a.begin() + l, a.begin() + m + 1);
    vector<int> R(a.begin() + m + 1, a.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)L.size() && j < (int)R.size()) {
        c++;
        if (L[i] <= R[j]) a[k++] = L[i++];
        else               { a[k++] = R[j++]; s++; }
    }
    while (i < (int)L.size()) a[k++] = L[i++];
    while (j < (int)R.size()) a[k++] = R[j++];
}

void mergeSort(vector<int>& a, int l, int r, long long& c, long long& s) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(a, l, m, c, s);
        mergeSort(a, m + 1, r, c, s);
        merge(a, l, m, r, c, s);
    }
}
// Benchmark 
// Runs sort 3 times on a fresh copy each time.
// Returns average time in SECONDS 
template<typename F>
double bench(F fn, vector<int> base, long long& c, long long& s) {
    double total = 0;
    for (int i = 0; i < 3; i++) {
        vector<int> a = base;
        long long tc = 0, ts = 0;
        auto st = high_resolution_clock::now();
        fn(a, tc, ts);
        auto en = high_resolution_clock::now();
        total += duration_cast<nanoseconds>(en - st).count() / 1e9;
        if (i == 2) { c = tc; s = ts; }   // keep last run's counts
    }
    return total / 3.0;
}

void printLine(char ch, int n) { cout << "  " << string(n, ch) << "\n"; }

void printTableHeader() {
    cout << "  " << left
         << setw(14) << "Input Case"
         << setw(16) << "Avg Time (s)"
         << setw(14) << "Comparisons"
         << "Swaps\n";
    printLine('-', 56);
}

void printRow(const string& caseName, double t, long long c, long long s) {
    cout << "  " << left
         << setw(14) << caseName
         << setw(16) << fixed << setprecision(5) << t
         << setw(14) << c
         << s << "\n";
}

int main() {

    //arrays
    vector<int> s5  = {1, 2, 3, 4, 5};
    vector<int> r5  = {5, 4, 3, 2, 1};
    vector<int> s100, r100;
    for (int i = 1;   i <= 100; i++) s100.push_back(i);
    for (int i = 100; i >= 1;   i--) r100.push_back(i);

    // Lambda wrappers
    auto sel = [](vector<int>& a, long long& c, long long& s){ selectionSort(a, c, s); };
    auto bub = [](vector<int>& a, long long& c, long long& s){ bubbleSort(a, c, s); };
    auto qck = [](vector<int>& a, long long& c, long long& s){ quickSort(a, 0, (int)a.size()-1, c, s); };
    auto mrg = [](vector<int>& a, long long& c, long long& s){ mergeSort(a, 0, (int)a.size()-1, c, s); };

    using F = function<void(vector<int>&, long long&, long long&)>;

    vector<pair<string, F>> algos = {
        {"Selection Sort", sel},
        {"Bubble Sort",    bub},
        {"Quick Sort",     qck},
        {"Merge Sort",     mrg}
    };

    vector<pair<string, vector<int>>> cases = {
        {"5  Sorted",   s5  },
        {"5  Reverse",  r5  },
        {"100 Sorted",  s100},
        {"100 Reverse", r100}
    };

    cout << "\n"; //Header
    cout << "  Empirical Analysis of Sorting Algorithms\n";
    cout << "  Student: Ibrar Ahmed Khan  |  Roll: 64292\n";
    printLine('=', 58);
    cout << "\n";

    for (auto& [algoName, algoFn] : algos) {// Results per algorithm 
        cout << "  >> " << algoName << "\n";
        printLine('-', 56);
        printTableHeader();
        for (auto& [caseName, caseData] : cases) {
            long long c = 0, s = 0;
            double t = bench(algoFn, caseData, c, s);
            printRow(caseName, t, c, s);
        }
        cout << "\n";
    }

    printLine('=', 58); //Time Complexity Summary 
    cout << "  Time Complexity Summary\n";
    printLine('-', 58);
    cout << "  " << left
         << setw(18) << "Algorithm"
         << setw(14) << "Best"
         << setw(14) << "Average"
         << "Worst\n";
    printLine('-', 58);
    cout << "  " << setw(18) << "Selection Sort" << setw(14) << "O(n^2)"    << setw(14) << "O(n^2)"    << "O(n^2)\n";
    cout << "  " << setw(18) << "Bubble Sort"    << setw(14) << "O(n)"      << setw(14) << "O(n^2)"    << "O(n^2)\n";
    cout << "  " << setw(18) << "Quick Sort"     << setw(14) << "O(nlogn)"  << setw(14) << "O(nlogn)"  << "O(n^2)\n";
    cout << "  " << setw(18) << "Merge Sort"     << setw(14) << "O(nlogn)"  << setw(14) << "O(nlogn)"  << "O(nlogn)\n";
    cout << "\n";
    
    printLine('=', 58);//  Space Complexity Summary 
    cout << "  Space Complexity Summary\n";
    printLine('-', 58);
    cout << "  " << left
         << setw(18) << "Algorithm"
         << setw(14) << "Space"
         << "In-Place\n";
    printLine('-', 58);
    cout << "  " << setw(18) << "Selection Sort" << setw(14) << "O(1)"     << "Yes\n";
    cout << "  " << setw(18) << "Bubble Sort"    << setw(14) << "O(1)"     << "Yes\n";
    cout << "  " << setw(18) << "Quick Sort"     << setw(14) << "O(log n)" << "Yes\n";
    cout << "  " << setw(18) << "Merge Sort"     << setw(14) << "O(n)"     << "No\n";
    cout << "\n";

    return 0;
}

