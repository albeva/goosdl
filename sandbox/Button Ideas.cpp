class Label;
class ImageView;
class Image;

/**
 * clickable button
 */
class Button : public View
{
public:
    
    /**
     * define how image and label are placed
     * in relation to one another.
     */
    enum class Placement {
        Default,
        ImageLabel,         // [image] space [Label]
        LabelImage,         // [Label] space [image]
        ImageBreakLabel,    // [image] newline [label]
        LabelBreakImage     // [label] newline [image]
    };
    
    /**
     * possible button states
     */
    enum class State {
        Default,            // default state applies to all
        Normal,             // normal state with no modifiers
        Hover,              // mouse hover
        Pressed,            // pressed down
        Active,             // marked active
        Disabled            // marked as disabled
    };
    
private:
    // the text label. can be null
    std::vector<Label *> m_label;
    
    // icon image
    std::vector<ImageView *> m_image;
    
    // background image
    std::vector<Image *> m_backgroundImage;
    
    // spacing around the image
    Inset m_imageInset;
    
    // spacing around the label
    Inset m_labelInset;
};