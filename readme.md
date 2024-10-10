# Setting up a Python Virtual Environment

This script will guide you through setting up a virtual environment for your Python project.

## Prerequisites:

* **Python 3:** Ensure you have Python 3.11+ installed on your system. 
* **Python 3 venv module:**  This should be included in your Python 3.11 installation.
* **G++**: For compiling C++ scripts
* **CPPYY:** For running C++ scripts on Python

## Steps (for Debian):

0. **Install python3-dev**
    ```
    apt install python3-dev

1. **Instal a virtual environment**
    ```bash
    apt install python3-venv

2. **Create the virtual environment:**
   ```bash
   python3 -m venv my-venv

3. **Switch to the virtual environment**
    ```bash
    source my-venv/bin/activate

4. **(Optional) Upgrade the pip**
    ```bash
    pip install --upgrade pip

5. **Install cppyy**
    ```bash
    pip install cppyy

6. **Install matplotlib library**
    ```bash
    pip install matplotlib