/*
 * We did not write this code entirely.
 * Part of this code comes from: https://www.geeksforgeeks.org/c-program-for-merge-sort.
 */

// Initialize stack pointer.
asm("li $sp, 0x23FFFFFC");

void mergeSort(int arr[], int l, int r);

int entry() { 
  int arr[] = {6, 5, -1, 4, -2, 7}; 
  int arr_size = 6;

  mergeSort(arr, 0, arr_size - 1);

  int result = 0;
  for (int i = 0; i < 6; i++) {
    result += arr[i] * (i + 1);
  }

  // -2 -1*2 + 4*3 + 5*4 + 6*5 + 7*6 = 100

  return result; 
} 

void merge(int arr[], int l, int m, int r) { 
  int i, j, k; 
  int n1 = m - l + 1; 
  int n2 =  r - m; 

  int L[n1], R[n2]; 

  for (i = 0; i < n1; i++) 
    L[i] = arr[l + i]; 
  for (j = 0; j < n2; j++) 
    R[j] = arr[m + 1+ j]; 

  i = 0; // Initial index of first subarray 
  j = 0; // Initial index of second subarray 
  k = l; // Initial index of merged subarray 
  while (i < n1 && j < n2) { 
    if (L[i] <= R[j]) { 
      arr[k] = L[i]; 
      i++; 
    } 
    else { 
      arr[k] = R[j]; 
      j++; 
    } 
    k++; 
  } 

  while (i < n1)  { 
    arr[k] = L[i]; 
    i++; 
    k++; 
  } 

  while (j < n2)  { 
    arr[k] = R[j]; 
    j++; 
    k++; 
  } 
} 
  
void mergeSort(int arr[], int l, int r) { 
  if (l < r) { 
    int m = l+(r-l)/2; 

    mergeSort(arr, l, m); 
    mergeSort(arr, m+1, r); 

    merge(arr, l, m, r); 
  } 
}
