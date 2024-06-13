## Install

```bash
python -m venv venv
```

linux:

```bash
source venv/bin/activate
```

from the root directory (currently `/web_map_demo/image_ws`):

```bash
pip install -r requirements.txt
```

deactivate when done working:

```bash
deactivate
```

Run the python file to generate json

```bash
python process_map.py
```

Cleanup generated files with

```bash
./cleanup.sh
```
