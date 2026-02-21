from setuptools import setup

package_name = 'smart_factory_task_server'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='you',
    maintainer_email='you@example.com',
    description='Fleet task server for multi-robot TurtleBot logistics demo',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'task_server = smart_factory_task_server.task_server:main',
        ],
    },
)