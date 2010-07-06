#ifndef PIPELINEELEMENT_H
#define PIPELINEELEMENT_H

#include <map>
#include <QString>
#include <QObject>

#include "RefPtr.h"

namespace plv
{
    class IInputPin;
    class IOutputPin;
    class Pipeline;

    typedef enum _PlvPipelineElementState
    {
        PLV_PLE_STATE_UNINITIALIZED,
        PLV_PLE_STATE_NOT_READY,
        PLV_PLE_STATE_READY
    } PlvPipelineElementState;

    class PipelineElement : public QObject, public RefCounted
    {
        Q_OBJECT

        typedef std::map< QString, RefPtr< IInputPin > > InputPinMap;
        typedef std::map< QString, RefPtr< IOutputPin > > OutputPinMap;

    protected:
        InputPinMap  m_inputPins;
        OutputPinMap m_outputPins;

    public:
        friend class Pipeline;

        /** QMetaType requires a public default constructor,
         *  a public copy constructor and a public destructor.
         */
        PipelineElement();
        PipelineElement( const PipelineElement& other );
        virtual ~PipelineElement();

        /** Initialise the element so it is ready to receive
          * process() calls.
          * Should this be reentrant?
          */
        virtual PlvPipelineElementState init() = 0;
        virtual PlvPipelineElementState checkConfig() = 0;

        void addInputPin( IInputPin* pin );
        void addOutputPin( IOutputPin* pin );

        IInputPin* getInputPin( const QString& name ) const;
        IOutputPin* getOutputPin( const QString& name ) const;

        /** @returns true when this PipelineElement is ready for procesing */
        virtual bool isReadyForProcessing() const;

        /** This function does the actual work of this PipelineElement and
          * is called by the PipelineScheduler when inputs of this processor
          * are ready i.e. when isReadyForProcessing returns true.
          */
        virtual void process() = 0;


    protected:
        RefPtr<Pipeline> m_parent;
        /**
         * This gets called by Pipeline when we are added to it.
         * Handles removing ourself from any previous pipeline we were part of
         * and sets m_parent to the new pipeline
         */
        virtual void setPipeline(Pipeline* parent);

        /**
          * private process function which handles scoping of input and output pins
          * and class the process() function of the super class.
          */
        void __process();
    };
}

#endif // PIPELINEELEMENT_H
