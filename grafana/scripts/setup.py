from setuptools import setup, find_packages

setup(
    name='esp32-data-collector',
    version='1.0.0',
    description='Fetches data from ESP32 sensor and stores it in InfluxDB',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    author='Conner Will',
    author_email='conner.will@example.com',
    packages=find_packages(),
    install_requires=[
        'requests',
        'influxdb'
    ],
    entry_points={
        'console_scripts': [
            'esp32_data_collector = main:main'
        ]
    },
)

