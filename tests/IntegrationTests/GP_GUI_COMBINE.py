import sys
import threading
from PyQt6.QtCore import Qt, QObject, QTimer, pyqtSignal, QCoreApplication
from PyQt6.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QPushButton, QWidget

# Import the "inputs" module for gamepad input (you may need to install it).
import inputs

class GamepadReader(QObject):
    gamepad_signal = pyqtSignal(dict)

    def __init__(self):
        super().__init__()
        self.running = True

    def run(self):
        while self.running:
            events = inputs.get_gamepad()
            for event in events:
                if event.code == 'ABS_Y':
                    self.LeftJoystickY = event.state
                elif event.code == 'ABS_X':
                    self.LeftJoystickX = event.state
                elif event.code == 'ABS_RY':
                    self.RightJoystickY = event.state
                elif event.code == 'ABS_RX':
                    self.RightJoystickX = event.state
                elif event.code == 'ABS_Z':
                    self.LeftTrigger = event.state
                elif event.code == 'ABS_RZ':
                    self.RightTrigger = event.state
                elif event.code == 'BTN_TL':
                    self.LeftBumper = event.state
                elif event.code == 'BTN_TR':
                    self.RightBumper = event.state
                elif event.code == 'BTN_SOUTH':
                    self.A = event.state
                elif event.code == 'BTN_NORTH':
                    self.Y = event.state #previously switched with X
                elif event.code == 'BTN_WEST':
                    self.X = event.state #previously switched with Y
                elif event.code == 'BTN_EAST':
                    self.B = event.state
                elif event.code == 'BTN_THUMBL':
                    self.LeftThumb = event.state
                elif event.code == 'BTN_THUMBR':
                    self.RightThumb = event.state
                elif event.code == 'BTN_SELECT':
                    self.Back = event.state
                elif event.code == 'BTN_START':
                    self.Start = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY1':
                    self.LeftDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY2':
                    self.RightDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY3':
                    self.UpDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY4':
                    self.DownDPad = event.state
            
            gamepad_state = [self.LeftJoystickX, self.LeftJoystickY]
            self.gamepad_signal.emit(gamepad_state)

    def stop(self):
        self.running = False

class MyMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Gamepad Reader Example")
        self.setGeometry(100, 100, 400, 200)

        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)

        self.layout = QVBoxLayout()

        self.label = QLabel("Press buttons on the gamepad:")
        self.layout.addWidget(self.label)

        self.button = QPushButton("Quit")
        self.button.clicked.connect(QCoreApplication.quit)
        self.layout.addWidget(self.button)

        self.central_widget.setLayout(self.layout)

        self.gamepad_reader = GamepadReader()
        self.gamepad_reader.gamepad_signal.connect(self.update_gamepad_data)

        self.thread = threading.Thread(target=self.gamepad_reader.run)
        self.thread.start()

    def update_gamepad_data(self, gamepad_state):
        # Update your GUI with gamepad data here
        self.label.setText(str(gamepad_state))

    def closeEvent(self, event):
        self.gamepad_reader.stop()
        self.thread.join()
        event.accept()

def main():
    app = QApplication(sys.argv)
    window = MyMainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
