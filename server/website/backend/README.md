## Install

from the root directory (currently `/server/map_demo/`):


Create python virtual environment:
```bash
python -m venv venv
```

Activate (linux):

```bash
source venv/bin/activate
```

```bash
pip install -r requirements.txt
```

deactivate when done working:

```bash
deactivate
```

Run the python file to generate json

```bash
python backend/process_map.py
```

Cleanup generated files with

```bash
./backend/cleanup.sh
```

Start simple python http server to test with:

```bash
cd /map_demo/website
python -m http.server -d website/
```
