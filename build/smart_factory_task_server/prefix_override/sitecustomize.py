import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/ashwenk/RS2_smart_factory/install/smart_factory_task_server'
