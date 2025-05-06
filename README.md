# 🚀 How to Run CUDA Programs in Google Colab

This guide walks you through compiling and executing CUDA (`.cu`) code in **Google Colab**. It's a simple way to leverage NVIDIA GPUs without needing a local CUDA setup.

---

## ✅ Step-by-Step Instructions

### 1️⃣ Enable GPU Support

1. Open your Google Colab notebook.
2. Go to **Runtime > Change runtime type**.
3. Set **Hardware accelerator** to `GPU` (preferably T4).
4. Click **Save**.

---

### 2️⃣ Check GPU Device Details

```bash
!nvidia-smi


### 3️⃣Save Your CUDA Code to a File

Use the `%%writefile` magic command in a new code cell to save your CUDA code:
%%writefile file_name.cu
// paste your CUDA code here


### 4️⃣  Compile the CUDA code
!nvcc -arch=sm_75 file_name.cu

### 5️⃣  Execute the compiled binary
!./a.out


### 6️⃣ Command to Run OpenMP Assignment
>  g++ -fopenmp Assignment1.cpp -o assign1                                                                                               
>  ./assign1
