FROM python:3.9-slim

# Set working directory
WORKDIR /app

# Copy requirements.txt into the container
COPY requirements.txt .

# Install dependencies from requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copy the Python script into the container
COPY fetch_data.py .

# Run the Python script
CMD ["python", "fetch_data.py"]
