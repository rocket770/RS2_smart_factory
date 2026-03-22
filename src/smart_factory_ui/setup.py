from setuptools import find_packages, setup

package_name = 'smart_factory_ui'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='nick',
    maintainer_email='skrillx789@hotmail.com',
    description='Smart Factory UI package',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'add_task_cli = smart_factory_ui.add_task_cli:main',
            'smart_factory_gui = smart_factory_ui.main:main',
        ],
    },
)