import omnific
import random

class omnific_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnific.get_component("Transform")
        scene_layer = omnific.get_scene_layer()
        collisions = scene_layer.get_collision_registry()
        win_events = omnific.query_event("Player won") + (omnific.query_event("AI won"))

        if len(win_events) > 0:
            ball_speed = win_events[0].get_parameters().numbers["ball_speed"]
            omnific.get_logger().write("ball_speed " + str(ball_speed))
            transform.translation.x = ball_speed * random.choice([-1.0, 1.0]) * 0.5
            transform.translation.y = ball_speed * random.choice([-1.0, 1.0]) * 0.5
        
        transform.translation.x *= 0.25
        transform.translation.y *= 0.25
        pass
    
