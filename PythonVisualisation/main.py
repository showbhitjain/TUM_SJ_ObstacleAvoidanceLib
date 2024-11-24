import numpy as np
import pandas as pd
from roboticstoolbox import DHRobot, RevoluteMDH
import json
import math
from spatialmath import SE3
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D


def read_json_file(filename):
    with open(filename, 'r') as file:
        # Parse the JSON data and convert to a dictionary
        data_dict = json.load(file)
    return data_dict


if __name__ == '__main__':
    # Read joint values from CSV and robot configuration from JSON files
    data = pd.read_csv("../output.csv", header=None)
    joint_values = data.values

    robot_config = read_json_file("../config/pandaRobot.json")
    Trajectory_config = read_json_file("../config/trajectoryConfig.json")
    mdh_params = robot_config["mdhParameters"]

    # Extract tool transformation parameters
    tool_rot = robot_config["displacementEEtoTCP"]["rotationXYZ"]
    tool_translation = robot_config["displacementEEtoTCP"]["translation"]
    tool_transformation = SE3(tool_translation) * SE3.RPY(tool_rot, order="zyx", unit="deg")

    # Create the robot using MDH parameters
    mdh_matrix = np.array(mdh_params)
    robot = DHRobot(
        [RevoluteMDH(d=mdh_matrix[i][1], a=mdh_matrix[i][2], alpha=math.radians(mdh_matrix[i][3])) for i in
         range(mdh_matrix.shape[0])],
        name="robot_arm",
        tool=tool_transformation
    )

    # Print robot information
    print(robot)

    # Create a plot to visualize the animation
    fig, ax = plt.subplots()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlim([-1, 1])
    ax.set_ylim([-1, 1])
    ax.set_zlim([0, 1])

    # Animate the robot movement
    ani = robot.plot(joint_values, block=False, movie="robot_motion.html")

    # Show the animation
    plt.show()

    # See PyCharm help at https://www.jetbrains.com/help/pycharm/
