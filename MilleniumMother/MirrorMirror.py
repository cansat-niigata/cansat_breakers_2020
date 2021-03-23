from RubberHuman import Runback,dist,act

if __name__ == '__main__':
    machine = Runback()

    machine.waitDrop().wire.purge()
    machine.updateGPS().moveForward().drill()
    while machine.updateGPS().getDistance_CoursetoGoal()['distance'] < 5:
        machine.facetoGoal().moveForward(8)
    while machine.updateGPS().getDistance_CoursetoGoal()['distance'] < 3:
        break#このあたりでカメラの処理
    machine.stream << 'Goal!' << dist.CFSOUT