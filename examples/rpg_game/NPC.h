/* date = January 10, 2025 4:35 PM */


#ifndef NPC_H
#define NPC_H
#include "ecs/EntityBuilder.h"

namespace RpgGame {

class NPC : public Pontilus::ECS::EntityBuilder {
public:
    // forward decl
    class NPCComponent;

    /**
     * A class dedicated to updating the current state of an NPC.
     */
    class NPCState {

    public:
        virtual ~NPCState() = default;

        virtual void start(NPCComponent *parent) = 0;
        virtual void update(NPCComponent *parent, double) = 0;
    };

    class NPCWaiting : public NPCState {
    public:
        void start(NPCComponent *parent) override;
        void update(NPCComponent *parent, double) override;
        static NPCWaiting *get();
    };
    class NPCTalking : public NPCState {
        /// Total time spent in the talking state.
        double timeTalking = 0;
        /// Time left until next character is displayed.
        double charCountdown = 0.1;
        /// Current char index into `NPCComponent::text[idx]` that will be displayed next.
        size_t charIdx = 0;
        /// Current text chunk being displayed in the textbox.
        size_t currentTextChunk = 0;

    public:
        void start(NPCComponent *parent) override;
        void update(NPCComponent *parent, double) override;
        static NPCTalking *get();
    };
    /**
     * The component backend for an NPC; handles state management and dynamic updating.
     */
    class NPCComponent : public Pontilus::ECS::Component {
    public:
        NPCComponent();
        int update(double dt) override;

        template<typename T>
        void changeState();


        /// Returns a "somewhat" unique name for the textbox entity to be spawned when interacting with this NPC.
        std::string getTBEntityName() const;

        void setTextChunk(const std::string &text, size_t idx);
        const std::string &getTextChunk(size_t idx) { return text[idx]; }
        void addTextChunk(const std::string &text);
        void removeTextChunk(size_t idx);

        size_t getNumTextChunks() const;

    private:
        /// Self-explanatory
        NPCState *state = nullptr;
        /// Text to display when talking with this NPC. Stored as a series of paragraphs.
        std::vector<std::string> text;
        /// Number of times this NPC has been interacted with.
        size_t numInteract;
    };
    void build(Pontilus::ECS::Entity *obj) override;
};

template<typename T>
void NPC::NPCComponent::changeState() {
    this->state = T::get();
    this->state->start(this);
}
} // RpgGame

#endif //NPC_H
