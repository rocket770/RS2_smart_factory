builtin_interfaces/Time stamp
string task_id
string type            # e.g. "pickup_dropoff"
int32 priority

geometry_msgs/PoseStamped pickup_pose
geometry_msgs/PoseStamped dropoff_pose

string assigned_robot   # empty if unassigned
string status           # "queued"|"assigned"|"enroute_pickup"|"enroute_dropoff"|"done"|"failed"