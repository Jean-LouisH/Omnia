import omnia

def on_logic_frame():
    movement_speed = 1.0
    input = omnia.get_input()
    transform = omnia.get_component("Transform")

    if input.is_pressed(["s", "dpad_down"]):
        transform.globally_translate_y(-movement_speed)
    elif input.is_pressed(["w", "dpad_up"]):
        transform.globally_translate_y(movement_speed)
    
