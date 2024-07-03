FROM python:3.9-slim

WORKDIR /app

RUN pip install requests influxdb

COPY fetch_data.py .

CMD ["python", "fetch_data.py"]
