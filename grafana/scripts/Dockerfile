FROM python:3.9-slim

WORKDIR /app

COPY fetch_data.py .

COPY setup.py .

RUN pip install --upgrade pip

RUN pip install --editable "${PWD}"

CMD ["python", "fetch_data.py"]
